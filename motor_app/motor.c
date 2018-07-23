#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <sys/stat.h>
#include <fcntl.h>
/* ioctl cmd */
#define MOTOR_STOP		0x1
#define MOTOR_RESET		0x2
#define MOTOR_MOVE		0x3
#define MOTOR_GET_STATUS	0x4
#define MOTOR_SPEED		0x5
#define MOTOR_GOBACK	0x6
#define MOTOR_CRUISE	0x7
/* directional_attr */
#define MOTOR_DIRECTIONAL_UP    0x0
#define MOTOR_DIRECTIONAL_DOWN    0x1
#define MOTOR_DIRECTIONAL_LEFT    0x2
#define MOTOR_DIRECTIONAL_RIGHT    0x3
#define MOTOR_INIT_SPEED    1000    /* unit :ns */

#define AXIS_HORIZONAL 1
#define AXIS_VERTICAL 2
#define AXIS_FORCIBLY 3

#include <arpa/inet.h>
#include <string.h>


typedef struct {
	int x;
	int y;
} motors_steps;

enum motor_status {
	MOTOR_IS_STOP,
	MOTOR_IS_RUNNING,
};

typedef struct  {
	int x;
	int y;
	enum motor_status status;
	int speed;
} motor_message;

typedef struct  {
	unsigned int x_max_steps;
	unsigned int y_max_steps;
	unsigned int x_cur_step;
	unsigned int y_cur_step;
} motor_reset_data;


void sendCommand(int cmd, void *buffer) {
    int fd = open("/dev/motor", O_WRONLY);
    ioctl(fd, cmd, buffer);
    close(fd);
}

void setSpeed(int speed) {
    int motorspeed = speed;
    sendCommand(MOTOR_SPEED, &motorspeed);
}

void setMovement(int direction, int steps) {

    int x = 0;
    int y = 0;
    switch(direction){
        case MOTOR_DIRECTIONAL_UP:
            y += 1;
        break;
        case MOTOR_DIRECTIONAL_DOWN:
            y -= 1;
        break;
        case MOTOR_DIRECTIONAL_LEFT:
            x -= 1;
        break;
        case MOTOR_DIRECTIONAL_RIGHT:
            x += 1;
        break;
    }
    motors_steps motor_move;
    motor_move.x = x;
    motor_move.y = y;
    int i = 0;
        for(i = 0;i<steps;i++){
            sendCommand(MOTOR_MOVE, &motor_move);
        }

}

void setStop() {
    sendCommand(MOTOR_STOP, 0);
}

void reset() {
    motor_reset_data motor_move;
    motor_move.x_max_steps = 0;
    motor_move.y_max_steps = 0;
    motor_move.x_cur_step = 0;
    motor_move.y_cur_step = 0;
    sendCommand(MOTOR_RESET, &motor_move);
}


void getStatus() {
    motor_message status;
    sendCommand(MOTOR_GET_STATUS, &status);
    printf("x: %d\n", status.x);
    printf("y: %d\n", status.y);
    printf("speed: %d\n", status.speed);


}
void cruise (){
    sendCommand(MOTOR_CRUISE, 0);
}

void calibrate() {
    reset();
}


int main(int argc, char *argv[]) {


    char direction = 0;
    int stepsize = 100;

    // Parse Arguments:
    int c;
    while ((c = getopt(argc, argv, "d:s:")) != -1) {
        switch (c) {
            case 'd':
                direction = optarg[0];
                break;
            case 's':
                stepsize = atoi(optarg);
                break;
            default:
                printf("Invalid Argument %c\n", c);
                exit(EXIT_FAILURE);
        }
    }


    switch (direction) {
        case 'u':
            setMovement(MOTOR_DIRECTIONAL_UP, stepsize);
            break;
        case 'd':
            setMovement(MOTOR_DIRECTIONAL_DOWN, stepsize);
            break;
        case 'l':
            setMovement(MOTOR_DIRECTIONAL_LEFT, stepsize);
            break;
        case 'r':
            setMovement(MOTOR_DIRECTIONAL_RIGHT, stepsize);
            break;
        case 'v':
            calibrate();
            break;
        case 'h':
            calibrate();
            break;
        case 's':
            setStop();
            break;
        case 'c':
            cruise();
            break;
        default:
            printf("Invalid Direction Argument %c\n", direction);
            getStatus();
            exit(EXIT_FAILURE);
    }
    getStatus();

    return 0;


}
