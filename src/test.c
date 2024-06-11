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
#define tact "/dev/tact"  // Tact Switch 디바이스 경로
#define COUNTDOWN_TIME 10 // 카운트다운 시간 (초)
#define MAX_CLCD_BYTES 32 // CLCD에 쓰는 최대 바이트 수

// 전역 변수
int clcds;
int fd_dipsw;
unsigned char dipsw_status;
int fd_dot;
unsigned char data[10];
unsigned char fnd_digits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
int fnd_dev;
int leds;
char led_array[] = { 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00};

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

    print("   Memory Game:      Press Start  ");

    // 딥스위치의 맨 오른쪽 버튼이 눌릴 때까지 기다립니다.
    do {
        read(fd_dipsw, &dipsw_status, 1);
    } while ((dipsw_status & 0x80) == 0);  // 맨 오른쪽 버튼이 눌리면 루프를 빠져나갑니다.
    write(leds, &led_array[7], sizeof(unsigned char)); // 맨 오른쪽 LED 켜기
    usleep(2000000);
    close(fd_dipsw);
}

// 모드 선택 화면 출력 함수
int displayMode(){
    int size;
    fd_dipsw = open(dip, O_RDWR);
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return -1;
    }
    print("Select Mode:        4x4 6x6 8x8  ");
    usleep(5000000);

    read(fd_dipsw, &dipsw_status, 1);
    // 왼쪽 4개의 딥스위치 상태를 추출합니다.
    unsigned char left_status = dipsw_status & 0x0F;

    // 왼쪽 딥스위치의 상태에 따라 dot matrix의 크기를 결정합니다.    
    if (left_status == 0x00) {
        size = 2;
    } else if (left_status == 0x01) {
        size = 4;
    } else if (left_status == 0x03) {
        size = 6;
    } else if (left_status == 0x07) {
        size = 8;
    }
    close(fd_dipsw);
    return size;
}

// 난이도 선택 화면 출력 함수
int displayLevel(){
    int num_points;
    fd_dipsw = open(dip, O_RDWR); // fd_dipsw 열기
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return -1;
    }
    print("Select Level:    Easy Normal Hard");
    usleep(5000000);

    read(fd_dipsw, &dipsw_status, 1);
    // 오른쪽 4개의 딥스위치 상태를 추출합니다.
    unsigned char right_status = dipsw_status & 0xF0;
    // 딥스위치의 상태에 따라 생성할 점의 개수를 결정합니다.
    if (right_status == 0x00) {
        num_points = 1;
    } else if (right_status == 0x10) {
        num_points = 3;
        write(leds, &led_array[0], sizeof(unsigned char)); // 첫 번째 LED 켜기
    } else if (right_status == 0x30) {
        num_points = 5;
        write(leds, &led_array[0], sizeof(unsigned char)); // 첫 번째 LED 켜기
        write(leds, &led_array[1], sizeof(unsigned char)); // 두 번째 LED 켜기
    } else if (right_status == 0x70) {
        num_points = 8;
        write(leds, &led_array[0], sizeof(unsigned char)); // 첫 번째 LED 켜기
        write(leds, &led_array[1], sizeof(unsigned char)); // 두 번째 LED 켜기
        write(leds, &led_array[2], sizeof(unsigned char)); // 세 번째 LED 켜기
    }
    close(fd_dipsw);
    return num_points;
}

void display_remaining_time(int remaining_seconds) {
    unsigned char fnd_data[4] = {0};

    // FND 장치 오픈
    if ((fnd_dev = open(fnd, O_RDWR)) < 0) {
        perror("FND open error");
        return;
    }

    // 남은 시간 계산 (초 단위)
    int seconds = remaining_seconds;

    // FND 데이터 구성
    fnd_data[0] = fnd_digits[seconds / 10];
    fnd_data[1] = 0x00; // 공백
    fnd_data[2] = fnd_digits[seconds % 10];
    fnd_data[3] = 0x00; // 공백

    // FND 장치에 데이터 출력
    write(fnd_dev, fnd_data, sizeof(fnd_data));

    // FND 장치 닫기
    close(fnd_dev);
}

// 패턴 카운트다운 함수
void countdown_pattern(void) {
    int remaining_seconds = COUNTDOWN_TIME;

    while (remaining_seconds >= 0) {
        display_remaining_time(remaining_seconds);
        usleep(1000000); // 1초 대기
        remaining_seconds--;
    }
}

unsigned char* randot(int size,int num_points) {
    fd_dot = open(dot, O_RDWR);
    if (fd_dot < 0) {
        printf("Device open error: %s\n", dot);
        return NULL;
    }
    while (1) {
        for (int i = 0; i < 10; i++) {
            data[i] = 0x00;
        }
        // 랜덤한 점을 켭니다.
        for (int j = 0; j < num_points; j++) {
            int row = rand() % size;  // 0~(size-1) 사이의 랜덤한 행 번호 생성
            int col = rand() % size;  // 0~(size-1) 사이의 랜덤한 열 번호 생성
            data[row] |= (0x80 >> col);  // 해당 위치의 LED 켜기
        }

        write(fd_dot, &data, 10);  // LED 상태 업데이트
        countdown_pattern();
        close(fd_dot);
    }
    return data;
}

int main(){
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Device open error: %s\n", led);
        exit(0);
    }
    beforeGame();
    int size = displayMode();
    int num_points = displayLevel();
    unsigned char* result = randot(size, num_points);
    close(leds);
}
