#define dip "/dev/dipsw"

int fd_dipsw;
unsigned char dipsw_status;

void beforeGame(){
    // 딥스위치 디바이스를 엽니다.
    fd_dipsw = open(dip, O_RDWR);
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return;
    }

    // 딥스위치의 맨 오른쪽 버튼이 눌릴 때까지 기다립니다.
    do {
        read(fd_dipsw, &dipsw_status, 1);
    } while ((dipsw_status & 0x80) == 0);  // 맨 오른쪽 버튼이 눌리면 루프를 빠져나갑니다.
    usleep(2000000);
    close(fd_dipsw);
}

int displayMode(){
    int size;
    fd_dipsw = open(dip, O_RDWR);
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return -1;
    }

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
    close(fd_dipsw);
}

// 난이도 선택 화면 출력 함수
int displayLevel(){
    int num_points;
    fd_dipsw = open(dip, O_RDWR); // fd_dipsw 열기
    if (fd_dipsw < 0) {
        printf("Device open error: %s\n", dip);
        return -1;
    }
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
    close(fd_dipsw);
}