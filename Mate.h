#ifndef Mate_h
#define Mate_h

#include "Config.h"

#define CACHE_SIZE 10             // Länge des Input Byte Buffers

class Mate
{
  public:
    Mate();

    byte receive();
    unsigned long timeout();
    void send(byte * data, byte numberOfElements);
    bool getMotEn();
    bool isKeeper();
    bool isRusher();
    byte getState();
    byte getScore();
  private:
    byte fetch();

    byte role = 0;
    byte state = 0;
    byte score = 0;
    byte cache[CACHE_SIZE]; // Zwischenspeicher für eingehende Bluetooth Nachrichten
    byte cacheIndex = 255;  // aktuelle Schreibposition im Zwischenspeicher
    byte distanceRight = 0;
    byte distanceFront = 0;
    byte distanceLeft = 0;
    byte distanceBack = 0;
    unsigned long responseTimer = 0;
};

#endif
