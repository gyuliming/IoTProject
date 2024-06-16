#define dot "/dev/dot"

int fd_dot;
unsigned char selected_points[10]; 

// 랜덤한 점을 Dot Matrix에 표시하는 함수
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

unsigned char* Correctanswer(int size, int num_points) {
    unsigned char* data = (unsigned char*)malloc(10 * sizeof(unsigned char));
    unsigned char tact_status;
    int row = 0, col = 0;
    int selected_count = 0;

    for (int i = 0; i < 10; i++) {
        data[i] = 0x00;
    }

    // 초기 위치 설정
    data[row] |= (0x80 >> col);

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

        fd_dot = open(dot, O_RDWR);
        if (fd_dot < 0) {
            printf("Device open error: %s\n", dot);
            return NULL;
        }
        write(fd_dot, data, 10);
        usleep(100000); // 0.1초 대기
        close(fd_dot);

}
