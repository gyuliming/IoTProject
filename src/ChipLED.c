#define led "/dev/led"

int leds;
char led_array[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

void beforeGame(){
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Device open error: %s\n", led);
        exit(0);
    }
    write(leds, &led_array[7], sizeof(unsigned char)); // 맨 오른쪽 LED 켜기
    close(leds);
}

int displayMode(){
    int size;
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Device open error: %s\n", led);
        exit(0);
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
    }
}

int displayLevel(){
    int num_points;
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Device open error: %s\n", led);
        exit(0);
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
}