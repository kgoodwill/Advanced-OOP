#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <typeinfo>
#include <string.h>


using namespace std;

typedef struct Deque Deque;
typedef struct Deque_Iterator Deque_Iterator;

/*Deque Class*/
/*Contains members and functions for a Deque*/
struct Deque{
	int *circularBuffer;
	int dequeSize;
	int capacity;
	int head;
	int tail;
	char type_name[6 + sizeof(int)] = "Deque_";

	void (* push_back)(Deque *, const int value);//
	void (* push_front)(Deque *, const int value);//
	void (* pop_back)(Deque *);//
	void (* pop_front)(Deque *);//
	bool (* empty)(Deque *);//Checks if circular buffer is empty
	size_t (*size)(Deque *);//Returns the size of the deque
	void (*clear)(Deque *);//Clears the deque
	bool (*cmp)(const int &, const int &);
	const int &(*front)(const Deque *);//Returns a ptr to the front of the deque
	const int &(*back)(const Deque *);//Returns a ptr to the back of the deque
	const int &(*at)(const Deque *, size_t t);//Returns a ptr to 
	Deque_Iterator (* begin)(const Deque *);
	Deque_Iterator (* end)(const Deque *);
	void (*dtor)(Deque *);

};

/*Other Function Declarations*/
int front_index(const Deque *myDeq);
int back_index(const Deque *myDeq);
void reallocate_array(Deque *myDeq);

/*Iterator for the Deque Class*/
/*Contains the necessary functions to iterate through an instance of the Deque class*/
struct Deque_Iterator{
	int current;
	const Deque *myDeq;

	void (* inc)(Deque_Iterator *);
	void (* dec)(Deque_Iterator *);
	const int &(*deref) (const Deque_Iterator *);
};

bool Deque_Iterator_equal(const Deque_Iterator &iter1, const Deque_Iterator &iter2){
	if(!(iter1.myDeq->cmp(iter1.deref(&iter1), iter2.deref(&iter2))) && !(iter2.myDeq->cmp(iter2.deref(&iter2), iter1.deref(&iter1)))){
		return true;
	}
	else{
		return false;
	}
}

bool Deque_equal(const Deque &myDeq1, const Deque &myDeq2){
	int current1 = front_index(&myDeq1);
	int current2 = front_index(&myDeq2);
	while (current1 != myDeq1.tail){
		if(!myDeq1.cmp(myDeq1.circularBuffer[current1], myDeq2.circularBuffer[current2]) && 
			!myDeq2.cmp(myDeq2.circularBuffer[current2], myDeq1.circularBuffer[current1])){
			if(current1 == myDeq1.capacity){
				current1 = 0;
			}
			else{
				current1++;
			}
			if(current2 == myDeq2.capacity){
				current2 = 0;
			}
			else{
				current2++;
			}
		}
		else{
			return false;
		}
	}
	if(current2 != myDeq2.tail){
		return false;
	}
	return true;
}

//Add memory to the current Deque
void reallocate_array(Deque *myDeq){
	cout << "reallocate_array()" << endl;
	int newCapacity = 2*(myDeq->capacity);
	int* array = (int*)malloc( newCapacity * sizeof(int));
	for(int i = 0; i < newCapacity - 1; i++){//Zero out array
		array[i] = 0;
	}
	int current = front_index(myDeq);
	for(int i = 0; i < myDeq->capacity; i++){
		if(current == myDeq->capacity){
			current = 0;
		}
		array[i] = myDeq->circularBuffer[current];
		current++;
	}
	myDeq->head = 0;
	myDeq->tail = current - 1;
	myDeq->circularBuffer = array;
	myDeq->capacity = newCapacity;
}

//Add element after the tail ptr
void push_back(Deque *myDeq, const int value){
	//Starting Case
	if(myDeq->circularBuffer[front_index(myDeq)] == 0){//Empty queue
		myDeq->circularBuffer[front_index(myDeq)] = value;	
	}
	else{
		int current;
		bool push_back = true;
		/*if(myDeq->dequeSize == myDeq->capacity){ //Full
			cout << "Here" << endl;
			reallocate_array(myDeq);
			push_back = false;
		}*/
		current = back_index(myDeq); //Start with current pointing at the tail
		while(push_back){
			if(current == myDeq->capacity){ //Circle around to the beginning
				current = 0;
			}
			else if(current == front_index(myDeq)){
				//cout << "Or Here" << endl;
				reallocate_array(myDeq);
				current++;
			}
			else if(myDeq->circularBuffer[current] == 0){
				myDeq->circularBuffer[current] = value;
				myDeq->tail = current;
				myDeq->dequeSize++;
				push_back = false;
			}
			else{
				current++;
			}
		}
	}
	cout << "push_back" << endl;
	for(int i = 0; i < myDeq->capacity; i++){
		cout << myDeq->circularBuffer[i] << " ";
 	}
	cout << endl;
	cout << myDeq->head << " : Head  " << myDeq->tail << " : Tail" << endl;
	cout << endl;
}

//Add element prior to the head ptr
void push_front(Deque *myDeq, int value){
	if(myDeq->circularBuffer[front_index(myDeq)] == 0){//Empty queue
		myDeq->circularBuffer[front_index(myDeq)] = value;	
	}
	else{
		int current;
		bool push_front = true;
		/*if(myDeq->dequeSize == myDeq->capacity){
			cout << "Here" << endl;
			reallocate_array(myDeq);
			push_front = false;
		}*/
		current = front_index(myDeq);
		while(push_front){
			if(current == 0){//If at the beginning
				current = myDeq->capacity - 1;
			}
			else if(current == back_index(myDeq)){
				//cout << "Or Here" << endl;
				reallocate_array(myDeq);
				current--;
			}
			else if(myDeq->circularBuffer[current] == 0){
				myDeq->circularBuffer[current] = value;
				myDeq->head = current;
				myDeq->dequeSize++;
				push_front = false;
			}
			else{
				current--;
			}
		}
	}
	cout << "push_front" << endl;
	for(int i = 0; i < myDeq->capacity; i++){
		cout << myDeq->circularBuffer[i] << " ";
 	}
 	cout << endl;
 	cout << myDeq->head << " : Head  " << myDeq->tail << " : Tail" << endl;
 	cout << endl;
}

//Remove element pointed at by the tail ptr
void pop_back(Deque *myDeq){
	//Check if array is empty
	if(myDeq->circularBuffer[front_index(myDeq)] == 0 && myDeq->circularBuffer[back_index(myDeq)] == 0){
		cout << "Deque is empty" << endl;
	}
	else{
		myDeq->circularBuffer[back_index(myDeq)] = 0;
		//cout << "Here" << endl;
		if(back_index(myDeq) == myDeq->capacity){
			myDeq->tail = 0;
		}
		else{
			myDeq->tail--;
		}
	}
	cout << "pop_back" << endl;
	for(int i = 0; i < myDeq->capacity; i++){
		cout << myDeq->circularBuffer[i] << " ";
 	}
 	cout << endl;
 	cout << myDeq->head << " : Head  " << myDeq->tail << " : Tail" << endl;
 	cout << endl;
}

//Remove element pointed at by the head ptr
void pop_front(Deque *myDeq){
	//Check if array is empty
	if(myDeq->circularBuffer[front_index(myDeq)] == 0 && myDeq->circularBuffer[back_index(myDeq)] == 0){
		cout << "Deque is empty" << endl;
	}
	else{
		myDeq->circularBuffer[front_index(myDeq)] = 0;
		//cout << "Here" << endl;
		if(front_index(myDeq) == myDeq->capacity - 1){
			myDeq->head = 0;
		}
		/*else if(myDeq->head == 0){
			myDeq->head = myDeq->capacity;
		}*/
		else{
			myDeq->head++;
			//cout << myDeq->head << " : Head" << endl;
		}
	}
	cout << "pop_front" << endl;
	for(int i = 0; i < myDeq->capacity; i++){
		cout << myDeq->circularBuffer[i] << " ";
 	}
 	cout << endl;
 	cout << myDeq->head << " : Head  " << myDeq->tail << " : Tail" << endl;
 	cout << endl;
}

//Returns if Deque is empty or not
bool empty(Deque *myDeq){
	return !myDeq->dequeSize;
}

//Returns the size of the Deque
size_t size(Deque *myDeq){
	return myDeq->dequeSize;
}

//Clears the circular array by resetting everything to the default values (leaves capacity the same)
void clear(Deque *myDeq){
	int current = front_index(myDeq);
	while(current != myDeq->tail + 1){
		myDeq->circularBuffer[current] = 0;
		current++;
	}
	myDeq->head = 0;
	myDeq->tail = 1;
	myDeq->dequeSize = 0;
}

//Returns the index pointed to by head
int front_index(const Deque *myDeq){
	if(myDeq->head == myDeq->capacity){
		return 0;
	}
	else{
		return myDeq->head;
	}
}

//Returns the index pointed to by tail
int back_index(const Deque *myDeq){
	if(myDeq->tail == 0){
		return myDeq->capacity;
	}
	else{
		return myDeq->tail;
	}
}

//Increase the iterator to the next element
void inc(Deque_Iterator *iter){
	if(iter->current == iter->myDeq->capacity){
		iter->current = 0;
	}
	else{
		iter->current++;
	}
}

//Decrease the iterator to the previous element
void dec(Deque_Iterator *iter){
	if(iter->current == 0){
		iter->current = iter->myDeq->capacity;
	}
	else{
		iter->current--;
	}
}

//Uses the at() to derefence the iterator
const int& deref(const Deque_Iterator *iter){
	return iter->myDeq->at(iter->myDeq, iter->current);
}

//Return the element at the front of the Deque
const int& front(const Deque *myDeq){
	return myDeq->circularBuffer[front_index(myDeq)];
}

//Return the element at the back of the Deque
const int& back(const Deque *myDeq){
	return myDeq->circularBuffer[back_index(myDeq)];
}

//Returns the element "at" the specific place in the Deque
const int& at(const Deque *myDeq, size_t t){
	cout << "Front: " << front_index(myDeq) << " t: " << t << "Capacity: " << myDeq->capacity << endl;
	cout << "At: " << front_index(myDeq + t) % myDeq->capacity << endl;
	return myDeq->circularBuffer[front_index(myDeq + t) % myDeq->capacity];
}

//Returns the iterator to the beginning of the Deque
Deque_Iterator begin(const Deque *myDeq){
	Deque_Iterator iter;
	iter.current = front_index(myDeq);
	iter.myDeq = myDeq;
	iter.inc = &inc; //FP
	iter.dec = &dec; //FP
	iter.deref = &deref; //FP
	return iter;
}

//Returns the iterator to the end of the Deque
Deque_Iterator end(const Deque *myDeq){
	Deque_Iterator iter;
	iter.current = back_index(myDeq);
	iter.myDeq = myDeq;
	iter.inc = &inc; //FP
	iter.dec = &dec; //FP
	iter.deref = &deref; //FP
	return iter;
}

void dtor(Deque *myDeq){
	free(myDeq->circularBuffer);
}

//Constructor
void Deque_ctor(Deque *myDeq, bool (*less)(const int &, const int &)){
	myDeq->circularBuffer = (int*)malloc(2*sizeof(int));
	myDeq->dequeSize = 0;
	myDeq->capacity = 2;
	myDeq->head = 0;
	myDeq->tail = 1;
	for(int i = 0; i < myDeq->capacity - 1; i++){//Zero out array
		myDeq->circularBuffer[i] = 0;
	}
	//strcat(myDeq->type_name, int)
	myDeq->size = &size;
	myDeq->empty = &empty;
	myDeq->clear = &clear;
	myDeq->push_back = &push_back;
	myDeq->push_front = &push_front;
	myDeq->pop_back = &pop_back;
	myDeq->pop_front = &pop_front;
	myDeq->front = &front;
	myDeq->back = &back;
	myDeq->cmp = less;
	myDeq->begin = &begin;
	myDeq->end = &end;
	myDeq->dtor = &dtor;
}

#endif