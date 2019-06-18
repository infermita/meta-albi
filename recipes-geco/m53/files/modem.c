#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <stdarg.h>
#include <linux/serial.h>
//#include <linux/ioctl.h>
//#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>
#define TIOCGRS485      0x542E
#define TIOCSRS485      0x542F

#define DEVICE "/dev/ttyUSB0"
#define SPEED B115200 

void do_log(const char *line, ...) {
    FILE *logfile;
    char *logname = "/var/log/bmgcoli.log";
    struct tm *ptr;
    time_t ttime;
    va_list ap;

    //if (logfname[0] == 0)
    //  return;
    logfile = fopen(logname, "a");

    if (!logfile)
        return;

    va_start(ap, line);
    ttime = time(NULL);
    ptr = localtime(&ttime);
    fprintf(logfile, "%04d/%02d/%02d %02d:%02d:%02d ", (ptr->tm_year) + 1900, (ptr->tm_mon) + 1, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
    vfprintf(logfile, line, ap);
    fprintf(logfile, "\n");
    fclose(logfile);

}

int openPort() {

    int COM/*,i,count*/;
    //char buffer[255];
    struct serial_rs485 rs485conf;
    struct termios ti;


    //if ((COM=open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
    //if ((COM = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | /*O_NONBLOCK |*/ O_NDELAY)) < 0) {
    if ((COM = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | /*O_NONBLOCK |*/ O_NDELAY)) < 0) {
        do_log("Impossibile aprire il device\n");
        return -1;
    } else {
        do_log("device aperto\n");

        /*
        do_log("terminale associato %s\n", ttyname(COM));

         tcgetattr(COM,&tty_attributes);
 
        // c_cflag
        // Enable receiver
        tty_attributes.c_cflag |= CREAD;        
 
        // 8 data bit
        tty_attributes.c_cflag |= CS8;          
 
        // c_iflag
        // Ignore framing errors and parity errors. 
        tty_attributes.c_iflag |= IGNPAR;       
 
        // c_lflag
        // DISABLE canonical mode.
        // Disables the special characters EOF, EOL, EOL2, 
        // ERASE, KILL, LNEXT, REPRINT, STATUS, and WERASE, and buffers 
        // by lines.
 
        // DISABLE this: Echo input characters.
        tty_attributes.c_lflag &= ~(ICANON);     
 
        tty_attributes.c_lflag &= ~(ECHO);      
 
        // DISABLE this: If ICANON is also set, the ERASE character 
        // erases the preceding input   
        // character, and WERASE erases the preceding word.
        tty_attributes.c_lflag &= ~(ECHOE);     
 
        // DISABLE this: When any of the characters INTR, QUIT, SUSP, 
        // or DSUSP are received, generate the corresponding signal.    
        tty_attributes.c_lflag &= ~(ISIG);      
 
        // Minimum number of characters for non-canonical read.
        tty_attributes.c_cc[VMIN]=1;            
 
        // Timeout in deciseconds for non-canonical read.
        tty_attributes.c_cc[VTIME]=0;           
 
        // Set the baud rate
        cfsetospeed(&tty_attributes,B115200);     
        cfsetispeed(&tty_attributes,B115200);
 
        tcsetattr(COM, TCSANOW, &tty_attributes);
 
        // Set RS485 mode: 
        rs485conf.flags = SER_RS485_ENABLED | SER_RS485_RTS_ON_SEND;
        rs485conf.delay_rts_before_send = 0;
        rs485conf.delay_rts_after_send = 0;
        if (ioctl (COM, TIOCSRS485, &rs485conf) < 0) {
            //printf("ioctl error\n");
        }
        
        //Quella forse OK
        struct serial_rs485 rs485conf;
        struct termios ti;
        speed_t speed;
        int status;
        
        //Set the port in 485 mode 
        rs485conf.flags |= SER_RS485_ENABLED;
        rs485conf.flags |= SER_RS485_RTS_ON_SEND;
        rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;
        rs485conf.delay_rts_before_send = 1;
        rs485conf.delay_rts_after_send = 1;
        
        rs485conf.flags |= SER_RS485_RX_DURING_TX;
        
        //status = ioctl(COM, TIOCSRS485, &ctrl485);
        status = ioctl(COM, TIOCSRS485, &rs485conf);
        if (status) {
            printf("%s: Unable to configure port in 485 mode, status (%i)\n", "ttyUSB0", status);
            //return -1;
        }
        //Fine quella forse OK
        
        int flags = fcntl(COM, F_GETFL);
        flags &= ~O_NONBLOCK;
        fcntl(COM, F_SETFL, flags);
        
        //Set the port speed 
        speed = B115200;
        tcgetattr(COM, &ti);
        cfsetospeed(&ti, speed);
        cfsetispeed(&ti, speed);
         */
        int flags = fcntl(COM, F_GETFL);
        flags &= ~O_NONBLOCK;
        fcntl(COM, F_SETFL, flags);
        
        ti.c_cflag = B115200 | CS8 | CLOCAL | CREAD; /**115200,8bits,1 stopbit,1start,no parity,recive enable */
        ti.c_iflag = IGNPAR;
        ti.c_oflag = 0;
        ti.c_lflag = 0;
        ti.c_cc[VTIME] = 0;
        ti.c_cc[VMIN] = 0;
        tcflush(COM, TCIFLUSH);
        if (tcsetattr(COM, TCSANOW, &ti) < 0) {
            printf("Failed to set Termios\n");
            return -1;
        }

        //tcsetattr(COM, TCSANOW, &ti);

        return COM;

    }
}

int openPort2() {

    struct termios tio; //to hold serial port settings
    struct termios stdio; //so we can accept user input
    struct termios old_stdio; //save the current port settings
    int tty_fd; //file descriptor for serial port
    int retval, res, n, res2, read1, wri;
    char buf[255];
    char buf2[255];


    tty_fd = open(DEVICE, O_RDWR | O_NDELAY | O_NOCTTY );
    if (tty_fd < 0) {
        perror(DEVICE);
        exit(-1);
    }
    printf("Init 1 complete.\n");

    tcflush(tty_fd, TCIOFLUSH);

    int f = fcntl(tty_fd, F_GETFL, 0);
    fcntl(tty_fd, F_SETFL, f & ~O_NDELAY);

    retval = tcgetattr(tty_fd, &old_stdio);
    if (retval != 0) {
        perror(DEVICE);
        exit(-1);
    }
    printf("Init 2 complete.\n");

    struct termios newtio;
    retval = tcgetattr(tty_fd, &newtio);
    if (retval != 0) {
        perror(DEVICE);
        exit(-1);
    }
    printf("Init 3 complete.\n");

    cfsetospeed(&newtio, SPEED);
    cfsetispeed(&newtio, SPEED);

    newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS8;
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~(PARENB | PARODD);
    newtio.c_cflag &= ~CRTSCTS;
    newtio.c_cflag &= ~CSTOPB;

    newtio.c_iflag = IGNBRK;
    newtio.c_iflag &= ~(IXON | IXOFF | IXANY);

    newtio.c_lflag = 0;

    newtio.c_oflag = 0;

    newtio.c_cc[VTIME] = 1;
    newtio.c_cc[VMIN] = 60;
    newtio.c_cc[VINTR] = 127;
    newtio.c_cc[VQUIT] = 28;
    newtio.c_cc[VERASE] = 8;
    newtio.c_cc[VKILL] = 21;
    newtio.c_cc[VEOF] = 4;
    newtio.c_cc[VSTOP] = 19;
    newtio.c_cc[VSTART] = 17;
    newtio.c_cc[VSUSP] = 26;
    newtio.c_cc[VREPRINT] = 18;
    //newtio.c_cc[VFLSH] = 15;
    newtio.c_cc[VWERASE] = 23;
    newtio.c_cc[VLNEXT] = 22;


    retval = tcsetattr(tty_fd, TCSANOW, &newtio);
    if (retval != 0) {
        perror(DEVICE);
        exit(-1);
    }
    printf("Init 4 complete.\n");

    int mcs = 0;
    ioctl(tty_fd, TIOCMGET, &mcs);
    mcs |= TIOCM_RTS;
    ioctl(tty_fd, TIOCMSET, &mcs);

    retval = tcgetattr(tty_fd, &newtio);
    if (retval != 0) {
        perror(DEVICE);
        exit(-1);
    }
    printf("Init 5 complete.\n");

    newtio.c_cflag &= ~CRTSCTS;

    retval = tcsetattr(tty_fd, TCSANOW, &newtio);
    if (retval != 0) {
        perror(DEVICE);
        exit(-1);
    }
    printf("Init 6 complete.\n");
    
    int flags = fcntl(tty_fd, F_GETFL);
    flags &= ~O_NONBLOCK;
    fcntl(tty_fd, F_SETFL, flags);
    
    return tty_fd;

}

int openPort3(){

    int USB = open( "/dev/ttyUSB0", O_RDWR| O_NONBLOCK | O_NDELAY );

    /* Error Handling */
    if ( USB < 0 )
    {
        //cout << "Error " << errno << " opening " << "/dev/ttyUSB0" << ": " << strerror (errno) << endl;
        perror("USB ");
    }

    /* *** Configure Port *** */
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if ( tcgetattr ( USB, &tty ) != 0 )
    {
        //cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
        perror("tcgerattr ");
    }

    /* Set Baud Rate */
    cfsetospeed (&tty, B115200);
    cfsetispeed (&tty, B115200);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;        // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    tty.c_cflag     &=  ~CRTSCTS;       // no flow control
    tty.c_lflag     =   0;          // no signaling chars, no echo, no canonical processing
    tty.c_oflag     =   0;                  // no remapping, no delays
    tty.c_cc[VMIN]      =   0;                  // read doesn't block
    tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout

    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
    tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
    tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    tty.c_oflag     &=  ~OPOST;              // make raw

    /* Flush Port, then applies attributes */
    tcflush( USB, TCIFLUSH );

    if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
    {
        //cout << "Error " << errno << " from tcsetattr" << endl;
    }
    
    return USB;

}
int openPort485() {

    int COM/*,i,count*/;
    struct termios ti;
    
    if ((COM = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        do_log("Impossibile aprire il device");
        return -1;
    } else {
	do_log("device aperto");
       
        if (set_interface_attribs(COM, B115200, 0) < 0) {
                do_log("error configuring the rs-485 port.\n");
                return -1;
        }else{
        
            return COM;
            
        }
        
        
        /* 
        int flags = fcntl(COM, F_GETFL);
        flags &= ~O_NONBLOCK;
        fcntl(COM, F_SETFL, flags);
        
        ti.c_cflag = B115200 | CS8 | CLOCAL | CREAD; //115200,8bits,1 stopbit,1start,no parity,recive enable 
        ti.c_iflag = IGNPAR;
        ti.c_oflag = 0;
        ti.c_lflag = 0;
        ti.c_cc[VTIME] = 0;
        ti.c_cc[VMIN] = 0;
        tcflush(COM, TCIFLUSH);
        if (tcsetattr(COM, TCSANOW, &ti) < 0) {
            do_log("Failed to set Termios\n");
            return -1;
        }else{
        
            return COM;
            
        }
	*/
        
    }



}

int openPort485_2(){

    int fd = open ("/dev/ttyRPC0", O_RDWR);
    if (fd < 0) {
        printf("Impossibile aprire il device\n");
        return 0;
    }else{
        struct serial_rs485 rs485conf;
        rs485conf.flags |= SER_RS485_ENABLED;
        //rs485conf.flags |= SER_RS485_RTS_ON_SEND;
        rs485conf.flags &= ~(SER_RS485_RTS_ON_SEND);
        
        //rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;
        rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
        //rs485conf.flags |= SER_RS485_RX_DURING_TX;
        if (ioctl (fd, TIOCSRS485, &rs485conf) < 0) {
            printf("Impossibile settare il device\n");
            return 0;
        }else{
            return fd;
        }
    }

}

int set_interface_attribs (int fd, int speed, int parity)
{
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0)
  {
    printf("error using tcgetattr\n");
    return -1;
  }

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK;         // ignore break signal
  tty.c_lflag = 0;                // no signaling chars, no echo,
                                  // no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 0;            // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
  {
    printf("error from tcsetattr\n");
    return -1;
  }
  return 0;
}
