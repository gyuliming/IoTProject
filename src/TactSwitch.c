#define tact "/dev/tactsw"  // Tact Switch 디바이스 경로

int tacts;
unsigned char data[10];
unsigned char selected_points[10]; 

unsigned char* Correctanswer(int size, int num_points) {
    tacts = open(tact, O_RDWR);
    if (tacts < 0) {
        printf("Device open error: %s\n", tact);
        return NULL;
    }

    unsigned char tact_status;
    int selected_count = 0;
    int row = 0, col = 0;

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

        // tact switch를 닫고 dot matrix를 열어 현 위치를 표시
        close(tacts);

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