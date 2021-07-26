#include <stdint.h>
#ifndef QUEUE_H
#define QUEUE_H
class Queue {
  private:
    uint16_t front;
    char* data;
    uint16_t capacity;
    uint16_t size;
  public:
    Queue(int capacity = 2048) 
    { 
      this->capacity = capacity;
      this->data = new char[capacity];  
      this->clear(); 
    }
    ~Queue() 
    {
      delete[] data;  
    }
    void push(const char item);
    char peek();
    char pop();
    inline uint16_t getSize()
    {
	return this->size;
    }


    inline uint16_t getCapacity()
    {
	return this->capacity;
    }
    void clear();
};
#endif
