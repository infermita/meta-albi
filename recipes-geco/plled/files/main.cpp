/* 
 * File:   main.cpp
 * Author: alberto
 *
 * Created on 25 febbraio 2016, 11.29
 */

#include <cstdlib>
#include <iostream>
#include <wiringPi.h>
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <map>
#include <sqlite3.h>
#include "Rs485.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#define PIN1 4
#define PIN2 5
#define PINTX 1

using namespace std;

namespace my {

    template < typename T > string to_string(const T& n) {
        ostringstream stm;
        stm << n;
        return stm.str();
    }
}

class PlData {
public:
    string name;
    time_t lastup;
    int fail;
    int val;

};

map<int, PlData> plMap;
Rs485 *rs;

void httpCall(string httpData) {

    try {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url("http://192.168.1.247/fsmomo/wserv.php"));
        request.setOpt(new curlpp::options::PostFields(httpData));
        request.setOpt(new curlpp::options::PostFieldSize(httpData.length()));
        request.perform(); 
    } 
    catch (curlpp::LogicError & e) {
        cout << e.what() << endl;
    } catch (curlpp::RuntimeError & e) {
        cout << e.what() << endl;
    }


}
int WriteData(string data,int h,int val,string st){

    string datar;
    cout << st << " a: " << data;
    digitalWrite(PINTX, 1);
    rs->Write((char *) data.c_str(), data.length());
    //usleep(100000);
    usleep(data.length()*1000+4000);
    digitalWrite(PINTX, 0);
    //usleep(100000);
    int ret= 0;
    //data.clear();
    if (rs->Read(datar)) {

        cout << "Ricevo: " << datar << "\n";

        if (datar.compare(plMap[h].name + ":OK") == 0) {

            plMap[h].fail = 0;
            plMap[h].lastup = time(NULL);
            plMap[h].val = val;
            ret = 1;

        } else {

            plMap[h].fail = 1;
            //plMap[h].lastup = time(NULL);


        }
    } else {
        plMap[h].fail = 1;
        //plMap[h].lastup = time(NULL);
    }
    
    return ret;

}
/*
 * 
 */
int main(int argc, char** argv) {

    
    int pl1, pl2, pl1Old, pl2Old, i, h;
    string data;
    char hostname[128];
    char statusPL[1];
    char *plVal;
    sqlite3 *db;
    time_t lastUpSt = time(NULL);
    int diffTime = 3600;
    int plCnt = 0;
    int start;
    time_t now;
    struct tm *now_tm;
    int hour;
    start = 1;
    /*
    if (fork()) {
        exit(1);
    }
    */
    pl1Old = -1;
    pl2Old = -1;

    PlData pldata;

    map<int, string> stName;

    stName[0] = "magistrini";
    stName[1] = "marconi";
    stName[2] = "boniperti";
    stName[3] = "squarini";
    
    

    for (i = 0; i < 4; i++) {

        pldata.name = stName[i]; //"rasp" + my::to_string(i);
        pldata.fail = 0;
        pldata.lastup = 0;
        pldata.val = 0;
        plMap[i] = pldata;

    }

    rs = new Rs485();

    geteuid();
    memset(hostname, 0, sizeof hostname);
    gethostname(hostname, sizeof hostname);
    wiringPiSetup();

    pinMode(PINTX, OUTPUT);

    if (strcmp("master", argv[1]) == 0) {

        pinMode(PIN1, INPUT);
        pinMode(PIN2, INPUT);

    } else {

        pinMode(PIN1, OUTPUT);
        pinMode(PIN2, OUTPUT);
    }

    digitalWrite(PINTX, 0);

    while (1) {

        //rs->Write("ciao come stai?",15);

        //std::cout << "Leggo: " << rs->Read() << std::endl;

        if (strcmp("master", argv[1]) == 0) {

            now = time(NULL);
            now_tm = localtime(&now);
            hour = now_tm->tm_hour;
            
            if(hour < 2 || hour > 5){
                
                start = 1;
            
                pl1 = digitalRead(PIN1);
                pl2 = digitalRead(PIN2);

                if (pl1 != pl1Old || pl2 != pl2Old || (lastUpSt + diffTime) < time(NULL)) {

                    sleep(1);

                    pl1 = digitalRead(PIN1);
                    pl2 = digitalRead(PIN2);

                    if (pl1 != pl1Old || pl2 != pl2Old || (lastUpSt + diffTime) < time(NULL)) {

                        if ((lastUpSt + diffTime) < time(NULL))
                            cout << "Comunico per tempo scaduto" << endl;

                        pl1Old = pl1;
                        pl2Old = pl2;
                        memset(statusPL, 0, sizeof statusPL);
                        statusPL[0] = (pl1 << 1) | pl2;

                        for (h = 0; h < plMap.size(); h++) {

                            data = plMap[h].name + ":" + statusPL[0] + "\n";

                            if(WriteData(data,h,statusPL[0],"Invio")==0){

                                WriteData(data,h,statusPL[0],"Reinvio");

                            }

                            usleep(100000);
                        }

                        for (h = 0; h < plMap.size(); h++) {

                            data = "PL:" + plMap[h].name + "-v:" + my::to_string(plMap[h].val) + "-l:" + my::to_string(plMap[h].lastup) + "-f:" + my::to_string(plMap[h].fail) + "\n";
                            //data = "sucaaaaaaaaaaaaaaa\n";
                            //cout << data;

                            digitalWrite(PINTX, 1);
                            rs->Write((char *) data.c_str(), data.length());
                            //sleep(1);
                            //usleep(100000);
                            usleep(data.length()*1000+4000);
                            digitalWrite(PINTX, 0);
			    data.clear();
                            rs->Read(data);
                            if (data.compare("PL:OK") == 0) {

                                cout << "Ricevuto: " << data << endl;

                            }

                            //sleep(1);
                            usleep(100000);

                        }
                        lastUpSt = time(NULL);
                    }

                }
            }else if(start==1){
                
                for (h = 0; h < plMap.size(); h++) {

                    statusPL[0] = 0;
                    
                    data = plMap[h].name + ":" + statusPL[0] + "\n";

                    if(WriteData(data,h,statusPL[0],"Invio per stop")==0){

                        WriteData(data,h,statusPL[0],"Reinvio per stop");

                    }

                    usleep(100000);
                }

                start = 0;
                
                
            
            }

            //data = "ciao";
            //rs->Write((char *)data.c_str(),data.length());

        } else /*if (strcmp("slave", argv[1]) == 0)*/ {

	    data.clear();

            if (rs->Read(data)) {

                cout << string(hostname) << " ricevo: " << data << endl;

                //plStr = strtok_r((char *)data.c_str(), searchT1,&plVal);
                int fi = data.find(":");
                if (string(hostname).compare(data.substr(0, fi)) == 0) {

                    if(data.substr(0, fi).compare(stName[3])==0){
                    
                        cout << "Resetto contatore plCnt" << endl;
                        plCnt = 0;
                        
                    }
                    
                    if (data.length() == string(hostname).length() + 2) {

                        strtok_r((char *) data.c_str(), ":", &plVal);

                        switch (plVal[0]) {

                            case 0:
                                digitalWrite(PIN1, LOW);
                                digitalWrite(PIN2, LOW);
                                break;
                            case 1:
                                digitalWrite(PIN1, LOW);
                                digitalWrite(PIN2, HIGH);
                                break;
                            case 2:
                                digitalWrite(PIN1, HIGH);
                                digitalWrite(PIN2, LOW);
                                break;
                            case 3:
                                digitalWrite(PIN1, HIGH);
                                digitalWrite(PIN2, HIGH);
                                break;

                        }
                        //sleep(2);
                        usleep(200000);
                        digitalWrite(PINTX, 1);
                        data = string(hostname) + ":OK\n";
                        cout << string(hostname) << " invio: " << data;
                        rs->Write((char *) data.c_str(), data.length());
                        //sleep(1);
                        //usleep(100000);
                        usleep(data.length()*1000+4000);
                        //usleep(840*data.length());
                        digitalWrite(PINTX, 0);
                    }else {
                        //sleep(2);
                        usleep(200000);
                        digitalWrite(PINTX, 1);
                        data = string(hostname) + ":KO\n";
                        cout << string(hostname) << " invio: " << data;
                        rs->Write((char *) data.c_str(), data.length());
                        //sleep(1);
                        usleep(100000);
                        //usleep(840*data.length());
                        digitalWrite(PINTX, 0);

                    }

                }

                if (data.substr(0, 2).compare("PL") == 0 && string(hostname).compare(stName[3]) == 0) {

                    sqlite3_open("/var/www/db", &db);

                    //PL:rasp0-v:0-l:1457363583-f:0
                    int p1 = data.find("-v");
                    int p2 = data.find("-l");
                    int p3 = data.find("-f");
                    int diff = 3;
                    string rasp = data.substr(3, p1 - diff);
                    string val = data.substr(p1 + diff, 1);
                    string date = data.substr(p2 + diff, p3 - p2 - diff);
                    string fail = data.substr(p3 + diff, 1);
                    string query = "REPLACE INTO pl values('" + rasp + "','" + val + "','" + date + "','" + fail + "')";

                    sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);

                    sqlite3_close(db);
                    cout << "Ras:" << rasp << " Val:" << val << " Data :" << date << " Fail: " << fail << endl;
                    //sleep(2);
                    usleep(200000);
                    digitalWrite(PINTX, 1);
                    data = data.substr(0, 2) + ":OK\n";
                    rs->Write((char *) data.c_str(), data.length());
                    //sleep(1);
                    usleep(100000);
                    digitalWrite(PINTX, 0);
                    plCnt++;

                    for (h = 0; h < plMap.size(); h++) {

                        if (plMap[h].name.compare(rasp) == 0) {

                            plMap[h].fail = atoi(fail.c_str());
                            plMap[h].val = atoi(val.c_str());
                            plMap[h].lastup = atol(date.c_str());

                        }

                    }

                }
                lastUpSt = time(NULL);


                if (plCnt == 4) {

                    string httpData = "{\"st\":[";

                    for (h = 0; h < plMap.size(); h++) {

                        httpData += "{\"nome\":\"" + plMap[h].name + "\",\"fail\":\"" + my::to_string(plMap[h].fail) + "\",\"val\":\"" + my::to_string(plMap[h].val) + "\",\"lastup\":\"" +my::to_string(plMap[h].lastup) + "\"}";

                        if (h < plMap.size() - 1)
                            httpData += ",";

                    }

                    httpData += "]}";

                    httpCall(httpData);
                    
                    plCnt = 0;
                }
                //if(strcmp(plStr,"PL")==0){



            } else if ((lastUpSt + diffTime) < time(NULL) && string(hostname).compare(stName[3]) == 0) {

                cout << "Setto fail per mancata comunicazione" << endl;

                sqlite3_open("/var/www/db", &db);
                string queryU = "UPDATE pl set fail='1';";
                sqlite3_exec(db, queryU.c_str(), NULL, NULL, NULL);
                sqlite3_close(db);
                lastUpSt = time(NULL);
            }/* else if ((lastUpSt + diffTime) < time(NULL)){
            
                cout << "Setto semofori per mancata comunicazione" << endl;
                digitalWrite(PIN1, LOW);
                digitalWrite(PIN2, LOW);
                lastUpSt = time(NULL);
            }*/

        }/* else {

            if (rs->Read(data)) {
                if (data.substr(0, 2).compare("PL") == 0) {
                    
                    sqlite3_open("/var/www/db", &db);
                    
                    //PL:rasp0-v:0-l:1457363583-f:0
                    int f =data.find("f:");
                    string rasp  = data.substr(3,5);
                    string val   = data.substr(11,1);
                    string date  = data.substr(15,10);
                    string fail  = data.substr(f+2,1);
                    string query = "REPLACE INTO pl values('"+rasp+"','"+val+"','"+date+"','"+fail+"')";
                    
                    sqlite3_exec(db,query.c_str(),NULL,NULL,NULL);
                    
                    sqlite3_close(db);
                    cout << "Ras:" << rasp << " Val:" << val << " Data :" << date << " Fail: " << fail << endl;
                    sleep(2);
                    digitalWrite(PINTX, 1);
                    data = data.substr(0, 2) + ":OK\n";
                    rs->Write((char *) data.c_str(), data.length());
                    sleep(1);
                    digitalWrite(PINTX, 0);

                }

            }


        }
        */
        usleep(100000);

    }

    delete rs;

    return 0;
}

