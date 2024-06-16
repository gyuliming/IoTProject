#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#define dot "/dev/dot"
#define clcd "/dev/clcd" // Character LCD
#define fnd "/dev/fnd"  // 7-Segment FND 디바이스 경로
#define dip "/dev/dipsw"
#define led "/dev/led"
#define tact "/dev/tactsw"  // Tact Switch 디바이스 경로
#define COUNTDOWN_TIME 10 // 카운트다운 시간 (초)
#define MAX_CLCD_BYTES 32 // CLCD에 쓰는 최대 바이트 수

// 전역 변수
int clcds;
int fd_dipsw;
int fd_dot;
int fnd_dev;
int leds;
int tacts;
int score = 0; // 점수 변수
unsigned char dipsw_status;
unsigned char data[10];
unsigned char fnd_digits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
char led_array[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
unsigned char selected_points[10]; 


// 입출력 함수
void print(const char *P){
  clcds = open(clcd, O_RDWR);
  if(clcds < 0){
    printf("Can't open Character LCD.\n");
    exit(0);
  }
  write(clcds, P, strnlen(P, MAX_CLCD_BYTES));
  close(clcds);
}

// 게임 시작 전 메뉴를 출력하는 함수
void beforeGame(){
    // 딥스위치 디바이스를 엽니다.
    fd_dipsw = open(dip, O_RDWR);
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return;
    }
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Device open error: %s\n", led);
        exit(0);
    }
    printf("beforeGame\n");

    print("Memory Game:    Press Start");

    // 딥스위치의 맨 오른쪽 버튼이 눌릴 때까지 기다립니다.
    do {
        read(fd_dipsw, &dipsw_status, 1);
    } while ((dipsw_status & 0x80) == 0);  // 맨 오른쪽 버튼이 눌리면 루프를 빠져나갑니다.
    write(leds, &led_array[7], sizeof(unsigned char)); // 맨 오른쪽 LED 켜기
    usleep(2000000);
    close(fd_dipsw);
    close(leds);
}

// 모드 선택 화면 출력 함수
int displayMode(){
    int size;
    fd_dipsw = open(dip, O_RDWR);
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return -1;
    }
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Device open error: %s\n", led);
        exit(0);
    }

    printf("displayMode\n");

    print("Select Mode:    4x4 6x6 8x8");
    usleep(3000000);

    read(fd_dipsw, &dipsw_status, 1);
    // 왼쪽 4개의 딥스위치 상태를 추출합니다.
    unsigned char left_status = dipsw_status & 0x0F;

    // 왼쪽 딥스위치의 상태에 따라 dot matrix의 크기를 결정합니다.    
    if (left_status == 0x00) {
        size = 2;
    } 
    else if (left_status == 0x01) {
        size = 4;
    } 
    else if (left_status == 0x03) {
        size = 6;
    } 
    else if (left_status == 0x07) {
        size = 8;
    }

    if (size == 4){
        write(leds, &led_array[0], sizeof(unsigned char)); // 첫 번째 LED 켜기
        usleep(1000000);
        close(leds);
    } 
    else if (size == 6){
        write(leds, &led_array[0], sizeof(unsigned char)); // 첫 번째 LED 켜기
        usleep(1000000);
        write(leds, &led_array[1], sizeof(unsigned char)); // 두 번째 LED 켜기
        usleep(1000000);
        close(leds);
    } 
    else if (size == 8){
        write(leds, &led_array[0], sizeof(unsigned char)); // 첫 번째 LED 켜기
        usleep(1000000);
        write(leds, &led_array[1], sizeof(unsigned char)); // 두 번째 LED 켜기
        usleep(1000000);
        write(leds, &led_array[2], sizeof(unsigned char)); // 세 번째 LED 켜기
        usleep(1000000);
        close(leds);
    close(fd_dipsw);
    return size;
    }
}

// 난이도 선택 화면 출력 함수
int displayLevel(){
    int num_points;
    fd_dipsw = open(dip, O_RDWR); // fd_dipsw 열기
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return -1;
    }
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Device open error: %s\n", led);
        exit(0);
    }

    printf("displayLevel\n");

    print("Select Level:   Easy Normal Hard");
    usleep(3000000);

    read(fd_dipsw, &dipsw_status, 1);
    // 오른쪽 4개의 딥스위치 상태를 추출합니다.
    unsigned char right_status = dipsw_status & 0xF0;
    // 딥스위치의 상태에 따라 생성할 점의 개수를 결정합니다.
    if (right_status == 0x00) {
        num_points = 1;
    } else if (right_status == 0x10) {
        num_points = 3;
    } else if (right_status == 0x30) {
        num_points = 5;
    } else if (right_status == 0x70) {
        num_points = 8;
    }
    if (num_points == 3){
        write(leds, &led_array[4], sizeof(unsigned char)); // 첫 번째 LED 켜기
        usleep(1000000);
        close(leds);
    } else if (num_points == 5){
        write(leds, &led_array[4], sizeof(unsigned char)); // 첫 번째 LED 켜기
        usleep(1000000);
        write(leds, &led_array[5], sizeof(unsigned char)); // 두 번째 LED 켜기
        usleep(1000000);
        close(leds);
    } else if (num_points == 8){
        write(leds, &led_array[4], sizeof(unsigned char)); // 첫 번째 LED 켜기
        usleep(1000000);
        write(leds, &led_array[5], sizeof(unsigned char)); // 두 번째 LED 켜기
        usleep(1000000);
        write(leds, &led_array[6], sizeof(unsigned char)); // 세 번째 LED 켜기
        usleep(1000000);
        close(leds);
    }
    close(fd_dipsw);
    return num_points;
}

unsigned char* randot(int size, int num_points) {
    fd_dot = open(dot, O_RDWR);
    if (fd_dot < 0) {
        printf("Device open error: %s\n", dot);
        return NULL;
    }
    printf("randot\n");

    unsigned char* data = (unsigned char*)malloc(10 * sizeof(unsigned char));
    for (int i = 0; i < 10; i++) {
        data[i] = 0x00;
    }

    // 랜덤한 점을 켭니다.
    for (int j = 0; j < num_points; j++) {
        int row = rand() % size;  // 0~(size-1) 사이의 랜덤한 행 번호 생성
        int col = rand() % size;  // 0~(size-1) 사이의 랜덤한 열 번호 생성
        data[row] |= (0x80 >> col);  // 해당 위치의 LED 켜기
    }

    write(fd_dot, data, 10);  // LED 상태 업데이트
    sleep(10);
    close(fd_dot);
    return data;
}


int check(unsigned char* result, unsigned char* correct) {
    printf("check\n");
    if (memcmp(result, correct, 10) == 0) {
        score += 50;
        char score_str[16];
        sprintf(score_str, "score : %d", score);
        if (score < 100) {
            print(score_str);
            sleep(3);
            return 1;
        } else {
            print("Congratulations!");
            sleep(3);
            return 0;
        }
    } else {
        print("Game    Over:   answer different");
        sleep(3);
        return 0;
    }
}

unsigned char* Correctanswer(int size, int num_points) {
    tacts = open(tact, O_RDWR);
    if (tacts < 0) {
        printf("Device open error: %s\n", tact);
        return NULL;
    }
    printf("Correctanswer\n");

    unsigned char* data = (unsigned char*)malloc(10 * sizeof(unsigned char));
    unsigned char tact_status;
    int row = 0, col = 0;
    int selected_count = 0;

    for (int i = 0; i < 10; i++) {
        data[i] = 0x00;
    }

    // 초기 위치 설정
    data[row] |= (0x80 >> col);

    while (selected_count < num_points) {
        read(tacts, &tact_status, 1);

        // 버튼 입력에 따라 점의 위치 이동
        if (tact_status == 2) { // 2번 버튼 (위로 이동)
            if (row > 0) {
                row--;
            }
        } else if (tact_status == 4) { // 4번 버튼 (왼쪽으로 이동)
            if (col > 0) {
                col--;
            }
        } else if (tact_status == 6) { // 6번 버튼 (오른쪽으로 이동)
            if (col < size - 1) {
                col++;
            }
        } else if (tact_status == 8) { // 8번 버튼 (아래로 이동)
            if (row < size - 1) {
                row++;
            }
        } else if (tact_status == 5) { // 1번 버튼 (선택)
            selected_points[selected_count++] = (row << 4) | col;
            data[row] ^= (0x80 >> col); // 선택한 점 토글
        }

        // 모든 위치를 지웁니다.
        for (int i = 0; i < 10; i++) {
            data[i] = 0x00;
        }

        // 선택한 위치를 표시합니다.
        for (int i = 0; i < selected_count; i++) {
            int r = selected_points[i] >> 4;
            int c = selected_points[i] & 0x0F;
            data[r] |= (0x80 >> c);
        }

        // 현재 위치를 표시합니다.
        data[row] |= (0x80 >> col);

        // tact switch를 닫고 dot matrix를 열어 현 위치를 표시
        close(tacts);
        fd_dot = open(dot, O_RDWR);
        if (fd_dot < 0) {
            printf("Device open error: %s\n", dot);
            return NULL;
        }
        write(fd_dot, data, 10);
        usleep(100000); // 0.1초 대기
        close(fd_dot);

        // 다시 tact switch를 엽니다.
        tacts = open(tact, O_RDWR);
        if (tacts < 0) {
            printf("Device open error: %s\n", tact);
            return NULL;
        }
    }
    close(tacts);
    return data;
}


int main(){
    while(1)
    {
        beforeGame();
        int size = displayMode();
        int num_points = displayLevel();
        unsigned char* result = randot(size, num_points);
        unsigned char* correct = Correctanswer(size, num_points);
        if(check(result, correct) == 1){
            continue;
        }
        else{
            break;
        }
        
    }
}