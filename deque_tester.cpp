//Deque Function Tester

#include "Deque.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

bool int_less(const int &a, const int &b){
	return a < b;
}

int main(){
	Deque deq;
	Deque_ctor(&deq, int_less);

	//Push Back Tests
	push_back(&deq, 1);
	push_back(&deq, 2);
	push_back(&deq, 3);
	push_back(&deq, 10);

	//Push Front Tests
	push_front(&deq, 4);
	push_front(&deq, 5);
	push_front(&deq, 6);

	push_back(&deq, 7);
	push_back(&deq, 8);

	push_front(&deq, 9);

	pop_back(&deq);
	pop_back(&deq);

	pop_front(&deq);
	pop_front(&deq);
}

