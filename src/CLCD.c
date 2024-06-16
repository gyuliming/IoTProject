#define clcd "/dev/clcd" // Character LCD
#define MAX_CLCD_BYTES 32 // CLCD에 쓰는 최대 바이트 수

int clcds;

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

// 게임 시작 전 메뉴
void beforeGame(){
    print("Memory Game:    Press Start");
}

// 모드 선택 
void displayMode(){
    print("Select Mode:    4x4 6x6 8x8");
}

// 난이도 선택
void displayLevel(){
    print("Select Level:   Easy Normal Hard");
}

// 결과 화면
void displayResult(){
    print("Congratulations!");
    print("Game    Over:   answer different");
}