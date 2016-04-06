/* 
 * File:   structs.h
 * Author: ellioth
 *
 * Created on April 2, 2016, 11:02 PM
 */

#ifndef STRUCTS_H
#define	STRUCTS_H

struct DataForThread{
    int _NumbreThread;
    void* _ThisClass;
};

struct DataForServers{
    int _port;
    const char* _IP;
    int _memory;
    int _priority;
    int _socketFd;
};
#endif	/* STRUCTS_H */

