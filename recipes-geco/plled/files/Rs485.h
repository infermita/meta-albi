/* 
 * File:   Rs485.h
 * Author: alberto
 *
 * Created on 25 febbraio 2016, 11.32
 */

#ifndef RS485_H
#define	RS485_H

#include <string>

using namespace std;

class Rs485 {
public:
    Rs485();
    Rs485(const Rs485& orig);
    virtual ~Rs485();
    int Read(string &ret);
    void Write(char *buff,int len);
private:
    int port;

};

#endif	/* RS485_H */

