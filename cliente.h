/* 
 * File:   cliente.h
 * Author: ellioth
 *
 * Created on March 22, 2016, 1:51 PM
 */

#ifndef CLIENTE_H
#define	CLIENTE_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "Constantes.h"
#include "structs.h"

using namespace rapidjson;
class cliente : public Constantes{
public:
    cliente();
    virtual ~cliente();
    char* readFromServer(int pNumbServer);
    void* writeOnServer(int pNumServer);
private:
    DataForServers _dataForServers[TOTAL_SERVERS];
    void readMemoryNodeDatas();
    void * connectToServer(int pNumThread);
    static void * starThread(void* datas){
        DataForThread temp= *((DataForThread*)datas);
        return ((cliente*)temp._ThisClass)->connectToServer(temp._NumbreThread);
    };
    void error(const char* msg);
    
};

#endif	/* CLIENTE_H */

