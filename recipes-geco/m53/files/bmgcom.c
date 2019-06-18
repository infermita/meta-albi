#include <gd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int com,cnt=0;
int addr = 0x8A;
int readPort() {

    char result[20];
    int count = 0;
    int wh = 3,ret = 0, i = 0;

    usleep(2500);

    //while(wh){
    //while(count<0){
  
	//usleep(60000); 
	        
        memset(result, 0x00, sizeof (result));

        count = read(com, result, sizeof (result));
	/*	 			
	if(count > 0){
        	do_log("Ricevo %d 0x%02x", count,result[0]);
	}/*else{
		printf("Non ricevo %d\n",wh);
	}*/
		
        for(i = 0; i < count;i++){

            if (result[i] == 0x15) {

                //printf("Ricevo %d NAK 0x%02x\n", count,result[i]);

                return 0;

            } else if (result[i] == 0x06) {
                //printf("Ricevo AK %d 0x%02x\n", count, result[i]);
                return 1;
            } else {

                printf("Ricevo BOO %d 0x%02x\n", count, result[i]);

            }
        }
        //usleep(1000); 
        //wh--;
        
    //}
    return ret;

}

unsigned char checksum(unsigned char *ptr, size_t sz) {
    unsigned char chk = 0;
    while (sz-- != 0)
        chk += *ptr++;
    
    chk |= 0x80; 
    
    return chk;
}

int sendBuff(unsigned char *buff,int size,int view){

    int i;

    for(i = 0; i < size;i++)
	write(com,buff+i,1);	

 //   printf("\n");

    //int wr = write(com,buff,size);
    
    //printf("Scrivo %d di %d\n",wr,size);
    
    int r = readPort();
    
    //printf("R %d cnt %d\n",r,cnt);
    
    if(r==0 && cnt < 3){
        //do_log("Riprovo %d %d",r,cnt);
        cnt++;
        sendBuff(buff,size,1);
    
    }else{
        
        if(cnt > 0 && r==0){
            cnt = 0;
            return 0;
            
        }else{
            cnt = 0;
            return 1;
            
        }
        
    }
}

int findChar(int c){

    int ret = 0,i;
    
    int charset[] = {0x02, 0x03, 0x04, 0x05, 0x10};
    //int charset[] = {0x02, 0x03, 0x04, 0x05};
    size_t size = (sizeof (charset) / sizeof (int));
    
    for(i = 0; i < size;i++){
    
        if(charset[i]==c){
            
            return 1;
        }
    
    }
    return ret;

}

void createbuf(int myAr[], int size) {

    int i,findChr = 0,cnt = 0;
    unsigned char *send,*sendT;

    send = malloc(size);

    memset(send, 0x00, sizeof (send));

    for (i = 0; i < size - 1; i++) {
        send[i] = myAr[i];
        if(findChar(myAr[i]) && i>=3 && i<size-2)
            findChr++;
        //printf("Send %d 0x%02x\n", i, send[i]);
    }

    //unsigned char checkSum = (checksum (&send[9], size - 10)+send[3]) | 0x80;
    unsigned char checkSum = checksum(&send[3], size - 4);
    send[size - 1] = checkSum;
    
    sendT = malloc(size+findChr);
    memset(sendT, 0x00, size+findChr);
    
    for(i = 0; i < size;i++){
    
        if(i == size-1 || i == size -2 || i <=2){
        
            sendT[cnt] = send[i];
        
        }else{
            if(findChar(send[i])){
            
                sendT[cnt] = 0x10;
                cnt++;
            }          
            sendT[cnt] = send[i];
            
        }
        cnt++;
    
    }
    
    sendBuff(sendT, size+findChr,0);
    //readPort(com);
    free(send);
    free(sendT);


}

void updateD() {
    //int myAr[] = {0x04, 0x81, 0x02, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
    int myAr[] = {0x04, addr, 0x02, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
    size_t size = (sizeof (myAr) / sizeof (int)) + 1;
    createbuf(myAr, size);
    

}

void deleteD() {

    //int linea[] = {0x04, 0x81, 0x02, 0x2f, 0x00, 0x00, 0x00, 0x03};
    int linea[] = {0x04, addr, 0x02, 0x2f, 0x00, 0x00, 0x00, 0x03};
    size_t size = (sizeof (linea) / sizeof (int)) + 1;
    createbuf(linea, size);

}

void partilImageUpdate(){

    int linea[] = {0x04, 0x81, 0x02, 0x3A, 0x00, 0x00, 0x00, 0x00,  0x00,0x00,  0x00,0x00,  0x00,0x28,  0x00,0xFF, 0x03};
    //int linea[] = {0x04, 0x81, 0x02, 0x3A, 0x00, 0x00, 0x80, 0x00,  0x00,0x00,  0x00,0x00,  0x00,0x28,  0x00,0x28, 0x03};
    //int linea[] = {0x04, addr, 0x02, 0x3A, 0x00, 0x00, 0x80, 0x00,  0x00,0x00,  0x00,0x00,  0x00,0x28,  0x00,0x28, 0x03};
    size_t size = (sizeof (linea) / sizeof (int)) + 1;
    createbuf(linea, size);
    
}

void updateD2(int addrP,int page) {

    com = openPort485();
    int myAr[] = {0x04, addrP, 0x02, 0x24, 0x00, 0x00, 0x00, page, 0x00, 0x03};
    size_t size = (sizeof (myAr) / sizeof (int)) + 1;
    createbuf(myAr, size);

    close(com);
    do_log("Aggiorno pagina %d indirizzo %d",page,addrP);

}

void writeImg(gdImagePtr im,int start,int h,int st,int addrP,int pageN){

    unsigned char buff[255];
    int x,y,h1,h2,w1,w2,red,green,blue,c,bmp,bmp1,bitsel;
    int height = h;//im->sy;
    int width = im->sx;
    addr = addrP;

    int numByte = width/8 + ( (width%8>0)?1:0);
    
    do_log("W %d H %d Byte %d Start %d", width, height,numByte,start);
    
    unsigned char resBit[numByte];
    
    w2 = width % 256;
    w1 = width / 256;
    
    com = openPort485();//openPort();
    
    //if(start==0)
    //    deleteD();
        //updateD();
        
    int hReal = 0;
    
    for (y = st; y < height; ++y) {

        int chCnt[400] = {};
        int ch = 7;
        
        memset(resBit,0,sizeof(resBit));
        memset(buff,0,sizeof(buff));
        
        h2 = hReal % 256;
        h1 = hReal / 256;
        hReal++;
        
        buff[0] = 0x04;
        buff[1] = addr;
        buff[2] = 0x02;
        buff[3] = 0x73;
        buff[4] = 0x00;
        buff[5] = 0x00;
        buff[6] = 0x00;
	/*
        buff[7] = pageN;
        buff[8] = 0x00;
        buff[9] = 0x00;
	*/
	if(findChar(pageN)){
            
            buff[ch] = 0x00;
            chCnt[0] = ch; 
            ch++;
            buff[ch] = pageN;
            
        }else{
            buff[ch] = pageN;
        }
        ch++;
        buff[ch] = 0x00;
        ch++;
        buff[ch] = 0x00;
        ch++;
        
        
        
        if(findChar(h1)){
        
            buff[ch] = 0x00;
            chCnt[1] = ch;
            ch++;
            buff[ch] = h1;
            ch++;
            
             
            
        }else{
        
            buff[ch] = h1;
            ch++;
            
        }
        
        if(findChar(h2)){
        
            buff[ch] = 0x00;
            chCnt[2] = ch; 
            ch++;
            buff[ch] = h2;
            ch++;
            
        }else{
        
            buff[ch] = h2;
            ch++;
            
        }
        
        if(findChar(w1)){
        
            buff[ch] = 0x00;
            chCnt[3] = ch; 
            ch++;
            buff[ch] = w1;
            ch++;
            
        }else{
        
            buff[ch] = w1;
            ch++;
            
        }
        
        if(findChar(w2)){
        
            buff[ch] = 0x00;
            chCnt[4] = ch; 
            ch++;
            buff[ch] = w2;
            ch++;
            
        }else{
        
            buff[ch] = w2;
            ch++;
            
        }
        
        for (x = 0; x < width; ++x) {

            int c = gdImageGetPixel(im, x, y);
    
            //printf("The value of the center pixel is %d\n", c);
            red = gdImageRed(im, c);
            green = gdImageGreen(im, c);
            blue = gdImageRed(im, c);
            
            //
            //printf(" RGB values are %d %d %d\n", red, green, blue);
            
            bmp = red | green | blue;
            
            bitsel = x/8;
            
            if (bmp >= 220) {
                resBit[bitsel] |= 0x80>>(x%8);
            }
            

        }
        
        int cntChcont = 4;
                
        for(bmp=0 ;bmp < numByte;bmp++){
        
            if(findChar(resBit[bmp])){
                
                buff[ch] = 0x00;
                chCnt[cntChcont] = ch;
                cntChcont++;
                ch++;
                
                
            }
            buff[ch] = resBit[bmp];
            ch++;
            
        
        }
        
        buff[ch] = 0x03;
                        
        unsigned char checkSum = checksum(&buff[3], ch);
        ch++;
        buff[ch] = checkSum;    
        ch++;
        
        for(bmp = 0; bmp < 400;bmp++){
        
            if(chCnt[bmp]> 0 )
                buff[chCnt[bmp]] = 0x10;
        
        }
        //do_log("Invio linea %d indirizzo 0x%02x",y,addr);
        if(sendBuff(buff, ch,0)==0){
        //sendBuff(buff, ch,0);
            do_log("Errore invio linea %d",y);
            //for(bmp1=0;bmp1< ch;bmp1++){
            //    printf("0x%02X ",buff[bmp1]);
            //}
            //printf("\n\n");
        }

    }

    //partilImageUpdate();
    //if(start==0){
    //    updateD();
    //}else{
    //    partilImageUpdate();
    //}
    //readPort();
    
    
    

    close(com);
    
    
}
void sendEnq(int addrP) {
    com = openPort485();
    int myAr[] = {0x04, addrP, 0x02, 0x6B, 0x00, 0x03};
    size_t size = (sizeof (myAr) / sizeof (int)) + 1;
    createbuf(myAr, size);
    close(com);
    do_log("Invio ENQ %d indirizzo",addrP);

}
void setConf(int addrP){
    com = openPort485();
    int myAr[] = {0x04, addrP, 0x02, 0x10, 0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x85};
    int i;
    size_t size = (sizeof (myAr) / sizeof (int));

     unsigned char *send;

    send = malloc(size);

    memset(send, 0x00, sizeof (send));

    for (i = 0; i < size ; i++) {
        send[i] = myAr[i];
        //printf("Send %d 0x%02x\n", i, send[i]);
    }


    //createbuf(myAr, size);
    sendBuff(send, size,0);
    close(com);

}
void getConf(int addrP){
   com = openPort485();
   int myAr[] = {0x04, addrP, 0x02, 0x38, 0x00, 0x03};
   size_t size = (sizeof (myAr) / sizeof (int)) + 1;
   createbuf(myAr, size);
   close(com);

}
