/* 
 * File:   cliente.cpp
 * Author: ellioth
 * 
 * Created on March 22, 2016, 1:51 PM
 */


#include "cliente.h"

cliente::cliente() {
    DataForThread datas;
    readMemoryNodeDatas();
    for(int i=0; i<TOTAL_SERVERS; i++){
        pthread_t hilo;
        datas._NumbreThread=i;
        datas._ThisClass=this;
        if(pthread_create(&hilo,NULL,&cliente::starThread,&datas)!=CERO){
            error(ERROR7);
        }
    }
}

cliente::~cliente() {
    
}

/**
 * metodo que lo usan los hilos para crear la conexion entre los servidores
 * que estan conectados.
 * @param pNumbThread recibe un dato tipo int, que es el numero del hilo 
 * que estamos usando.
 * @return no retorna nada, solo se usa para la instancia del thread.
 */
void* cliente::connectToServer(int pNumbThread) {
    struct sockaddr_in _serv_addr;
    struct hostent * _server;
    _dataForServers[pNumbThread]._socketFd=socket(AF_INET, SOCK_STREAM, CERO);
    if (_dataForServers[pNumbThread]._socketFd < CERO) 
        error(ERROR1);
    _server = gethostbyname(_dataForServers[pNumbThread]._IP);
    if (_server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(CERO);
    }
    bzero((char *) &_serv_addr, sizeof(_serv_addr));
    _serv_addr.sin_family = AF_INET;
    bcopy((char *)_server->h_addr, 
        (char *)&_serv_addr.sin_addr.s_addr,
        _server->h_length);
    _serv_addr.sin_port = htons(_dataForServers[pNumbThread]._port);
    if (connect(_dataForServers[pNumbThread]._socketFd,
            (struct sockaddr *) &_serv_addr,sizeof(_serv_addr))<
            CERO) 
        error(ERROR4);
}

/**
 * metodo para obtener los datos de los nodos de memoria de los 
 * servers, obtiene los datos y los establece en una estructura de
 */
void cliente::readMemoryNodeDatas() {
    //abrimos el archivo
    std::fstream docmuent_reader(FILE_SERVERS);
    if(docmuent_reader.is_open()){
        //sacamos el largo del archivo
        docmuent_reader.seekg(CERO,std::ios::end);
        int sizeOfFile= docmuent_reader.tellg();
        //creamos una variable para guardar los datos del archivo
        char JsonData[sizeOfFile+UNO];
        docmuent_reader.seekg(CERO);
        //leemos los datos del archivo
        docmuent_reader.read(JsonData,sizeOfFile);
        //creamos el json y parseamos el char
        Document JsonReader;
        JsonReader.Parse((const char*)JsonData);
        //ciclo para sacar los datos de todos los servers
        for(int i=0; i<TOTAL_SERVERS; i++){
            //creamos la varible que tiene un json anidado con los datos
            std::string serverN= "server"+std::to_string(i);
            Value& listOfDatas= JsonReader[serverN.c_str()];
            //Json anidado y lo parseamos
            Document JsonAnidado;
            assert(listOfDatas.IsArray());
            Value& JsonStringAnidado= listOfDatas[CERO];
            JsonAnidado.Parse(JsonAnidado.GetString());
            //variable que obtendra los datos del json
            Value & JsonOfDatas= JsonAnidado["ip"];
            _dataForServers[i]._IP= JsonOfDatas.GetString();
            JsonOfDatas= JsonAnidado["port"];
            _dataForServers[i]._port= JsonOfDatas.GetInt();
            listOfDatas= JsonAnidado["memory"];
            _dataForServers[i]._memory=JsonOfDatas.GetInt();
            listOfDatas= JsonAnidado["priority"];
            _dataForServers[i]._priority=JsonOfDatas.GetInt();
        }
    }
}

char* cliente::readFromServer(int pNumbServer) {

}

void* cliente::writeOnServer(int pNumServer) {

}

/**
 * metodo para botar todo el programa si existe algun fallo y 
 * evitar errores futuros.
 * @param msg dato char* que es el mensaje que corresponde a error 
 * probocado.
 */
void cliente::error(const char* msg) {
    perror(msg);
    exit(UNO);
}