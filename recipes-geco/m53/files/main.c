/* 
 * File:   main.c
 * Author: alberto
 *
 * Created on 12 febbraio 2014, 10.27
 */

#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <gdfontl.h>
#include <gdfontt.h>
#include <gdfonts.h>
#include <gdfontmb.h>
#include <gdfontg.h>
#include <pthread.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


//Data section
int QNumRow = 0;
time_t found = 0;
int continueA = 1;
int maxpage = 0;
//Struct Data

int enableB = 0;

//Draw section
char TheFont[] = "/opt/bmg/BmgColistericgdi/arial.ttf";
int startPointer = 0;
int numDrawRow = 0;
int numMaxRow = 23;
int numPage;
int curPage = 1;

void thSleep(void *ptr){

    int cnt = 10;
    
    while(cnt){
    
        do_log("ThreadSleep %d",cnt);
    
        cnt--;
        sleep(1);
    }
    
    loading("Riparte ciclo automatico");
    
    sleep(2);
    
    found = 0;
    continueA = 1;

}

void *readIO(void *ptr) {

    int ret,avv,ind;
    geteuid();
    wiringPiSetup();
    pinMode(5, INPUT);
    pinMode(4, INPUT);
    time_t last = 0;
    int step = 120;
    
    pthread_t threadSleep = 0;

    sleep(7);
    
    for (;;) {

	if(enableB){

       
	    avv = digitalRead(4);
            ind = digitalRead(5);



            if(avv){

		continueA++;

                if(continueA==maxpage){
                    continueA = 0x00;
                }

                do_log("Cliccato avanti presento pagina: %d", continueA);

                updateD2(0x0A,continueA);
                updateD2(0x0B,continueA);

		last = time(NULL);

                sleep(4);

            }

            if(ind){

		continueA--;

                if(continueA<0){
                    continueA = maxpage-1;
                }

                do_log("Cliccato indietro presento pagina: %d", continueA);

                updateD2(0x0A,continueA);
                updateD2(0x0B,continueA);

		last = time(NULL);

                sleep(4);



            } 
	    if (time(NULL) >= last + step) {

                if (continueA != 0) {

                    do_log("Riparto dalla prima pagina");

                    continueA = 0x00;
                    updateD2(0x0A, continueA);
                    updateD2(0x0B, continueA);
                    sleep(4);

                }

            }
        
        
        } 
        usleep(50000);

    }

}

int main(int argc, char** argv) {

    int i,change = 0,start = 0;
    struct stat sb; 
    pthread_t threadIO;
    
    //if(fork()){
    //    do_log("Demone attivato");
    //    exit(1);
   // }
    do_log("/********     Avvio PROG     *******/\n\n"); 
    pthread_create( &threadIO, NULL, readIO, NULL);
    pthread_detach(threadIO);
    setConf(0x0A);
    setConf(0x0B);
    drawTotal(0);
   
    while(1){
    
	if(stat("/m53image/update",&sb)==0)
        {
	    enableB = 0;
            do_log("File update trovato");
            remove("/m53image/update");
            drawTotal(0);
        }
        sleep(1);
        
    }
    
    return 0;
}



