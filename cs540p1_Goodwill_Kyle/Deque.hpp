#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <typeinfo>
#include <string.h>


using namespace std;

#define Deque_DEFINE(Class)																				\
																										\
typedef struct Deque_##Class Deque_##Class;																\																				\
typedef struct Deque_##Class##_Iterator Deque_##Class##_Iterator;										\
																										\
/*Deque Class*/																							\
/*Contains members and functions for a Deque*/															\
struct Deque_##Class{																					\
	int *circularBuffer;																				\
	int dequeSize;																						\
	int capacity;																						\
	int head;																							\
	int tail;																							\
	char type_name[6 + sizeof(#Class)] = "Deque_";														\
																										\
	void (* push_back)(Deque_##Class *, const Class value);												\
	void (* push_front)(Deque_##Class *, const Class value);											\
	void (* pop_back)(Deque_##Class *);																	\
	void (* pop_front)(Deque_##Class *);																\
	bool (* empty)(Deque_##Class *);																	\
	size_t (*size)(Deque_##Class *);																	\
	void (*clear)(Deque_##Class *);																		\
	bool (*cmp)(const Class &, const Class &);															\
	const Class &(*front)(const Deque_##Class *);														\
	const Class &(*back)(const Deque_##Class *);														\
	const Class &(*at)(const Deque_##Class *, size_t t);												\
	Deque_##Class##_Iterator (* begin)(const Deque_##Class *);											\
	Deque_##Class##_Iterator (* end)(const Deque_##Class *);											\
	void (*dtor)(Deque_##Class *);																		\
};																										\
																										\
/*Other Function Declarations*/																			\
int front_index(const Deque_##Class *myDeq);															\
int back_index(const Deque_##Class *myDeq);																\
void reallocate_array(Deque_##Class *myDeq);															\
																										\
/*Iterator for the Deque Class*/																		\
/*Contains the necessary functions to iterate through an instance of the Deque class*/					\
struct Deque_##Class##_Iterator{																		\
	int current;																						\
	const Deque_##Class *myDeq;																			\
																										\
	void (* inc)(Deque_##Class##_Iterator *);															\
	void (* dec)(Deque_##Class##_Iterator *);															\
	const Class &(*deref) (const Deque_##Class##_Iterator *);											\
};																										\
																										\
bool Deque_##Class##_Iterator_equal(const Deque_##Class##_Iterator &iter1, const Deque_##Class##_Iterator &iter2){						\
	if(!(iter1.myDeq->cmp(iter1.deref(&iter1), iter2.deref(&iter2))) && !(iter2.myDeq->cmp(iter2.deref(&iter2), iter1.deref(&iter1)))){	\
		return true;																													\
	}																																	\
	else{																																\
		return false;																													\
	}																																	\
}																																		\
																																		\
bool Deque_##Class##_equal(const Deque_##Class &myDeq1, const Deque_##Class &myDeq2){					\
	int current1 = front_index(&myDeq1);																\
	int current2 = front_index(&myDeq2);																\
	while (current1 != myDeq1.tail){																	\
		if(!myDeq1.cmp(myDeq1.circularBuffer[current1], myDeq2.circularBuffer[current2]) && 			\
			!myDeq2.cmp(myDeq2.circularBuffer[current2], myDeq1.circularBuffer[current1])){				\
			if(current1 == myDeq1.capacity){															\
				current1 = 0;																			\
			}																							\
			else{																						\
				current1++;																				\
			}																							\
			if(current2 == myDeq2.capacity){															\
				current2 = 0;																			\
			}																							\
			else{																						\
				current2++;																				\
			}																							\
		}																								\
		else{																							\
			return false;																				\
		}																								\
	}																									\
	if(current2 != myDeq2.tail){																		\
		return false;																					\
	}																									\
	return true;																						\
}																										\
																										\
/*Add memory to the current Deque*/																		\
void reallocate_array(Deque_##Class *myDeq){															\
	int newCapacity = 2*(myDeq->capacity);																\
	int* array = (int*)malloc( newCapacity * sizeof(Class));												\
	for(int i = 0; i < newCapacity - 1; i++){															\
		array[i] = 0;																					\
	}																									\
	int current = front_index(myDeq);																	\
	for(int i = 0; i < myDeq->capacity; i++){															\
		if(current == myDeq->capacity){																	\
			current = 0;																				\
		}																								\
		array[i] = myDeq->circularBuffer[current];														\
		current++;																						\
	}																									\
	myDeq->head = 0;																					\
	myDeq->tail = current - 1;																			\
	myDeq->circularBuffer = array;																		\
	myDeq->capacity = newCapacity;																		\
}																										\
																										\
/*Add element after the tail ptr*/																		\
void push_back(Deque_##Class *myDeq, const Class value){												\
	if(myDeq->circularBuffer[front_index(myDeq)] == 0){													\
		myDeq->circularBuffer[front_index(myDeq)] = value;												\
	}																									\
	else{																								\
		int current;																					\
		bool push_back = true;																			\
		current = back_index(myDeq);																	\
		while(push_back){																				\
			if(current == myDeq->capacity){																\
				current = 0;																			\
			}																							\
			else if(current == front_index(myDeq)){														\
				reallocate_array(myDeq);																\
				current++;																				\
			}																							\
			else if(myDeq->circularBuffer[current] == 0){												\
				myDeq->circularBuffer[current] = value;													\
				myDeq->tail = current;																	\
				myDeq->dequeSize++;																		\
				push_back = false;																		\
			}																							\
			else{																						\
				current++;																				\
			}																							\
		}																								\
	}																									\
}																										\
																										\
/*Add element prior to the head ptr*/																	\
void push_front(Deque_##Class *myDeq, Class value){														\
	if(myDeq->circularBuffer[front_index(myDeq)] == 0){													\
		myDeq->circularBuffer[front_index(myDeq)] = value;												\
	}																									\
	else{																								\
		int current;																					\
		bool push_front = true;																			\
		current = front_index(myDeq);																	\
		while(push_front){																				\
			if(current == 0){																			\
				current = myDeq->capacity - 1;															\
			}																							\
			else if(current == back_index(myDeq)){														\
				reallocate_array(myDeq);																\
				current--;																				\
			}																							\
			else if(myDeq->circularBuffer[current] == 0){												\
				myDeq->circularBuffer[current] = value;													\
				myDeq->head = current;																	\
				myDeq->dequeSize++;																		\
				push_front = false;																		\
			}																							\
			else{																						\
				current--;																				\
			}																							\
		}																								\
	}																									\
}																										\
																										\
/*Remove element pointed at by the tail ptr*/															\
void pop_back(Deque_##Class *myDeq){																	\
	if(myDeq->circularBuffer[front_index(myDeq)] == 0 && myDeq->circularBuffer[back_index(myDeq)] == 0){\
		cout << "Deque is empty" << endl;																\
	}																									\
	else{																								\
		myDeq->circularBuffer[back_index(myDeq)] = 0;													\
		if(back_index(myDeq) == myDeq->capacity){														\
			myDeq->tail = 0;																			\
		}																								\
		else{																							\
			myDeq->tail--;																				\
		}																								\
	}																									\
}																										\
																										\
/*Remove element pointed at by the head ptr*/															\
void pop_front(Deque_##Class *myDeq){																	\
	if(myDeq->circularBuffer[front_index(myDeq)] == 0 && myDeq->circularBuffer[back_index(myDeq)] == 0){\
		cout << "Deque is empty" << endl;																\
	}																									\
	else{																								\
		myDeq->circularBuffer[front_index(myDeq)] = 0;													\
		if(front_index(myDeq) == myDeq->capacity - 1){													\
			myDeq->head = 0;																			\
		}																								\
		else{																							\
			myDeq->head++;																				\
		}																								\
	}																									\
}																										\
																										\
/*Returns if Deque is empty or not*/																	\
bool empty(Deque_##Class *myDeq){																		\
	return !myDeq->dequeSize;																			\
}																										\
																										\
/*Returns the size of the Deque*/																		\
size_t size(Deque_##Class *myDeq){																		\
	return myDeq->dequeSize;																			\
}																										\
																										\
/*Clears the circular array by resetting everything to the default values (leaves capacity the same)*/	\
void clear(Deque_##Class *myDeq){																		\
	int current = front_index(myDeq);																	\
	while(current != myDeq->tail + 1){																	\
		myDeq->circularBuffer[current] = 0;																\
		current++;																						\
	}																									\
	myDeq->head = 0;																					\
	myDeq->tail = 1;																					\
	myDeq->dequeSize = 0;																				\
}																										\
																										\
/*Returns the index pointed to by head*/																\
int front_index(const Deque_##Class *myDeq){															\
	if(myDeq->head == myDeq->capacity){																	\
		return 0;																						\
	}																									\
	else{																								\
		return myDeq->head;																				\
	}																									\
}																										\
																										\
/*Returns the index pointed to by tail*/																\
int back_index(const Deque_##Class *myDeq){																\
	if(myDeq->tail == 0){																				\
		return myDeq->capacity;																			\
	}																									\
	else{																								\
		return myDeq->tail;																				\
	}																									\
}																										\
																										\
/*Increase the iterator to the next element*/															\
void inc(Deque_##Class##_Iterator *iter){																\
	if(iter->current == iter->myDeq->capacity){															\
		iter->current = 0;																				\
	}																									\
	else{																								\
		iter->current++;																				\
	}																									\
}																										\
																										\
/*Decrease the iterator to the previous element*/														\
void dec(Deque_##Class##_Iterator *iter){																\
	if(iter->current == 0){																				\
		iter->current = iter->myDeq->capacity;															\
	}																									\
	else{																								\
		iter->current--;																				\
	}																									\
}																										\
																										\
/*Uses the at() to derefence the Iterator*/																\
const Class& deref(const Deque_##Class##_Iterator *iter){												\
	return iter->myDeq->at(iter->myDeq, iter->current);													\
}																										\
																										\
/*Return the element at the front of the Deque*/														\
const Class& front(const Deque_##Class *myDeq){															\
	return myDeq->circularBuffer[front_index(myDeq)];													\
}																										\
																										\
/*Return the element at the back of the Deque*/															\
const Class& back(const Deque_##Class *myDeq){															\
	return myDeq->circularBuffer[back_index(myDeq)];													\
}																										\
																										\
/*Returns the element "at" the specific place in the Deque*/											\
const Class& at(const Deque_##Class *myDeq, size_t t){													\
	cout << "Front: " << front_index(myDeq) << " t: " << t << "Capacity: " << myDeq->capacity << endl;	\
	cout << "At: " << front_index(myDeq + t) % myDeq->capacity << endl;									\
	return myDeq->circularBuffer[front_index(myDeq + t) % myDeq->capacity];								\
}																										\
																										\
/*Returns the iterator to the beginning of the Deque*/													\
Deque_##Class##_Iterator begin(const Deque_##Class *myDeq){												\
	Deque_##Class##_Iterator iter;																		\
	iter.current = front_index(myDeq);																	\
	iter.myDeq = myDeq;																					\
	iter.inc = &inc;																					\
	iter.dec = &dec;																					\
	iter.deref = &deref;																				\
	return iter;																						\
}																										\
																										\
/*Returns the iterator to the end of the Deque*/														\
Deque_##Class##_Iterator end(const Deque_##Class *myDeq){												\
	Deque_##Class##_Iterator iter;																		\
	iter.current = back_index(myDeq);																	\
	iter.myDeq = myDeq;																					\
	iter.inc = &inc;																					\
	iter.dec = &dec;																					\
	iter.deref = &deref;																				\
	return iter;																						\
}																										\
																										\
void dtor(Deque_##Class *myDeq){																		\
	free(myDeq->circularBuffer);																		\
}																										\
																										\
void Deque_ctor(Deque_##Class *myDeq, bool (*less)(const Class &, const Class &)){						\
	myDeq->circularBuffer = (int*)malloc(2*sizeof(Class));												\
	myDeq->dequeSize = 0;																				\
	myDeq->capacity = 2;																				\
	myDeq->head = 0;																					\
	myDeq->tail = 1;																					\
	for(int i = 0; i < myDeq->capacity - 1; i++){														\
		myDeq->circularBuffer[i] = 0;																	\
	}																									\
	strcat(myDeq->type_name, #Class);																	\
	myDeq->size = &size;																				\
	myDeq->empty = &empty;																				\
	myDeq->clear = &clear;																				\
	myDeq->push_back = &push_back;																		\
	myDeq->push_front = &push_front;																	\
	myDeq->pop_back = &pop_back;																		\
	myDeq->pop_front = &pop_front;																		\
	myDeq->front = &front;																				\
	myDeq->back = &back;																				\
	myDeq->cmp = less;																					\
	myDeq->begin = &begin;																				\
	myDeq->end = &end;																					\
	myDeq->dtor = &dtor;																				\
}

#endif