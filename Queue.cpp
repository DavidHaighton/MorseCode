#include "Queue.h"

void Queue::push(const char item)
{
	if(this->getSize()==this->getCapacity())
	{
		return;
	}
	
	register uint16_t end = (this->front+this->size)%this->capacity;
	this->data[end] = item;
	
	this->size++;
}

char Queue::pop() 
{
	if(this->getSize()==0)
	{
		return '\0';
	}
	char val = peek();
	this->front = (this->front+(uint16_t)1)%this->capacity;
	this->size--;
	return val;
}

char Queue::peek() 
{
	return this->getSize()==0 ? '\0' : this->data[this->front];
}

void Queue::clear() 
{
	this->front=(uint16_t)0;
	this->size=(uint16_t)0;
}
