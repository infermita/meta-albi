

#include <gd.h>
#include <gdfontl.h>
#include <gdfontt.h>
#include <gdfonts.h>
#include <gdfontmb.h>
#include <gdfontg.h>
#include <dirent.h>
//#include "dao.h"

//extern trList trlist[];
extern int startPointer,numDrawRow,enableB,continueA,maxpage;
extern char TheFont[];


void loading(char *title){

    
    do_log(title);
    
    gdImagePtr im, dst;
    int intestFont = 30;
     
    im = gdImageCreate(640, 40);
    dst = gdImageCreate(640, 800);
    
    gdImageColorAllocate(im, 255, 0, 0);
    gdImageColorAllocate(dst, 0, 0, 0);
    
    int bk = gdImageColorAllocate(im, 0, 0, 0);
    
    gdImageStringFT(im, NULL, bk, TheFont, intestFont, 0, 12, 32, (char *) title);
    
    gdImageCopyRotated(dst, im, 620, 400, 0, 0, 800, 40, 270);
    
    //FILE *out;
    
    //out = fopen("loaging.gif","wb+");
    
    //gdImageGif(dst, out);
    
    writeImg(im,1);
    
    gdFree(dst);
    gdFree(im);
    
    //fclose(out);
    
}
void list_dir(int start) {
    
    struct dirent **namelist;
    int i, n;
    FILE *fp;
    //gdImagePtr im;
    char file[256];
    char path[]= "/m53image/";
    
    maxpage = 0;
    n = scandir(path, &namelist, 0, alphasort);
    if (n < 0)
        do_log("Directory %s non trovata",path);
    else {
        for (i = 0; i < n; i++) {
            
            if ( !strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, "..") )
            {
                // do nothing (straight logic)
            } else {
            
                memset(file,0,sizeof(file));
                
                sprintf(file,"%s%s",path,namelist[i]->d_name);
		//if(maxpage==3){
                fp = fopen(file, "r");
                gdImagePtr im;
                im = gdImageCreateFromJpeg(fp);
                fclose(fp);

                do_log("Invio parte 1 %s",file);
                writeImg(im, start, 800, 0, 0x0A, maxpage);
                updateD2(0x0A, maxpage);
                do_log("Invio parte 2 %s",file);
                writeImg(im, start, 1600, 800, 0x0B, maxpage);
                updateD2(0x0B, maxpage);
                //gdFree(im);
                gdImageDestroy(im);
                //} 
                maxpage++;
            
            }
            free(namelist[i]);
        }
        //gdImageDestroy(im);
        
	updateD2(0x0A, 0);
	updateD2(0x0B, 0);
        continueA = 0;
	do_log("Maxpage %d Continue %d",maxpage,continueA);	
    
    }
    free(namelist);
}
void drawTotal(int start) {

    list_dir(start); 
    /* 
    gdImagePtr im, dst;
    int i;
    int startRect = 40;
    int Hrow = 50;
    int intestFont = 30;
    int y = Hrow + startRect;
    int inc = 30;
    int fontH = 20;
    FILE *fp;

    list_dir();
    return;
    
    fp = fopen("Partenze_1.jpg","r");
    im = gdImageCreateFromJpeg(fp);
    fclose(fp);
   
    do_log("Invio parte 1 Partenze_1.jpg"); 
    writeImg(im,start,800,0,0x8A,0x00);
    updateD2(0x8A,0);
    do_log("Invio parte 2 Partenze_1.jpg");
    writeImg(im,start,1600,800,0x8B,0x00);
    updateD2(0x8B,0);
    gdFree(im);
    
    fp = fopen("prova_partenze_1.jpg","r");
    im = gdImageCreateFromJpeg(fp);
    fclose(fp);
    
    do_log("Invio parte 1 prova_partenze_1.jpg");
    writeImg(im,start,800,0,0x8A,0x01);
    updateD2(0x8A,0x01);
    do_log("Invio parte 2 prova_partenze_1.jpg");
    writeImg(im,start,1600,800,0x8B,0x01);
    updateD2(0x8B,0x01);
    gdFree(im);
    */ 
    enableB = 1; 

    //gdImageGif(dst, out);
    //gdFree(dst);
    

}
/*
void drawTotal2(int start) {

    gdImagePtr im, dst;
    int i;
    int startRect = 40;
    int Hrow = 50;
    int intestFont = 30;
    int y = Hrow + startRect;
    int inc = 30;
    int fontH = 20;

    //im = gdImageCreate(640, 800);
    im = gdImageCreate(640, 800);
    
    gdImageColorAllocate(im, 255, 255, 255);

    int bk = gdImageColorAllocate(im, 0, 0, 0);

    gdImageStringFT(im, NULL, bk, TheFont, intestFont, 0, 12, 32, (char *) "Partenze - Stazione di Iseo");

    int intStIntFont = (Hrow + startRect)-((Hrow - intestFont) / 2);


    gdImageRectangle(im, 10, startRect, 120, Hrow + startRect, bk);
    gdImageRectangle(im, 120, startRect, 520, Hrow + startRect, bk);
    gdImageRectangle(im, 520, startRect, 630, Hrow + startRect, bk);

    gdImageStringFT(im, NULL, bk, TheFont, intestFont, 0, 12, intStIntFont, (char *) "Treno");
    gdImageStringFT(im, NULL, bk, TheFont, intestFont, 0, 122, intStIntFont, (char *) "Destinazione");
    gdImageStringFT(im, NULL, bk, TheFont, intestFont, 0, 522, intStIntFont, (char *) "Ora");

    
    for (i = startPointer; i < numDrawRow; i++) {

        do_log("Creo riga%d",i);
        
        gdImageRectangle(im, 10, y, 120, y+30, bk);
        gdImageRectangle(im, 120, y, 520, y+30, bk);
        gdImageRectangle(im, 520, y, 630, y+30, bk);

        gdImageStringFT(im, NULL, bk, TheFont, 20, 0, 12, (y + inc)-((inc - fontH) / 2), (char *) trlist[i].nuTr);
        gdImageStringFT(im, NULL, bk, TheFont, 20, 0, 122, (y + inc)-((inc - fontH) / 2), (char *) trlist[i].stazione);
        gdImageStringFT(im, NULL, bk, TheFont, 20, 0, 522, (y + inc)-((inc - fontH) / 2), (char *) trlist[i].ora);

        y += inc;

    }
    
    writeImg(im,start);
    

    //gdImageGif(dst, out);
    gdFree(im);

}
*/