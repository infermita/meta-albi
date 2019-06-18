/* 
 * File:   Rs485.cpp
 * Author: alberto
 * 
 * Created on 25 febbraio 2016, 11.33
 */

#include "Rs485.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <iostream>
#include <sstream>

//#define DEVICE "/dev/ttyUSB0"
#define DEVICE "/dev/ttyAMA0"
#define SPEED B9600 

Rs485::Rs485() {

    struct termios ti, options;
    int status;

    if ((port = open(DEVICE, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) < 0) {
        //do_log("Impossibile aprire il device\n");
        std::cout << "Porta non aperta" << std::endl;
        port = -1;
    } else {

        std::cout << "Porta aperta" << std::endl;


        int flags = fcntl(port, F_GETFL);
        flags &= ~O_NONBLOCK;
        fcntl(port, F_SETFL, flags);

        ti.c_cflag = SPEED | CS8 | CLOCAL | CREAD; //115200,8bits,1 stopbit,1start,no parity,recive enable 
        ti.c_iflag = IGNPAR;
        ti.c_oflag = 0;
        ti.c_lflag = 0;
        ti.c_cc[VTIME] = 40;
        ti.c_cc[VMIN] = 0;
        tcflush(port, TCIFLUSH);
        if (tcsetattr(port, TCSANOW, &ti) < 0) {
            //do_log("Failed to set Termios\n");
            port = -1;
            std::cout << "Porta setattr errore" << std::endl;
        }
        

    }

}

Rs485::Rs485(const Rs485& orig) {
}

Rs485::~Rs485() {
    std::cout << "Chiudo porta" << std::endl;
    close(port);
}

int Rs485::Read(string &ret) {

    char result[50];
    char readB;
    int cnt, i, c, r;
    memset(result, 0, sizeof (result));
    cnt = r = 0;
    while (1) {
        cnt += read(port, &readB, 1);
        if (cnt) {

            if (readB == '\n')
                break;

            result[r] = readB;
            r++;
        }else{
        
            break;
        }
    }
    if(cnt){
    //    printf("letto%s\n", result);
        ret = string(result,cnt -1);
    }
    
    return cnt;

}

void Rs485::Write(char *buff, int len) {

    int lenw = write(port, buff, len);
    
    //printf("Inviato: %d\n",lenw);

}

