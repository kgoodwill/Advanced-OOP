//Map Class

#ifndef MAP_HPP
#define MAP_HPP

#include <utility>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <stdio.h>
#include <stdlib.h>

namespace cs540{
	template <typename Key, typename Value>
	class Map{
		private:
			class Node;
		public:
			class Iterator;
			class ConstIterator;
			class ReverseIterator;
		private:
			class Node{
				public:
					std::pair<const Key, Value> *n_value;
					Node* left; //BST Left
					Node* right; //BST Right
					Node* parent; //BST Parent
					Node* prev; //DLL Previous
					Node* next; //DLL Next

					Node() : prev(nullptr), next(nullptr) {}
					Node(std::pair<const Key, Value> n_value_in, Node* parentIn) : n_value(new std::pair<const Key, Value>(n_value_in)), left(nullptr), right(nullptr), parent(nullptr), prev(nullptr), next(nullptr){}
					Key key();
					Value value();
					Node *get_before();
					Node *get_after();
			};
			Node* head;
			Node* tail;
			std::size_t m_size;

			Iterator insert(Node* n, Node* insert_me);

		public:
			class Iterator{
				public:
					Iterator(const Iterator&);
					Iterator& operator=(const Iterator&);
					Iterator(Iterator&&);
					Iterator& operator=(Iterator&&);
					~Iterator();
					Iterator& operator++();
					Iterator operator++(int);
					Iterator& operator--();
					Iterator operator--(int);
					std::pair<const Key, Value>& operator*() const;

					bool operator==(const Iterator& iter);
					bool operator!=(const Iterator& iter);
				private:
					Node* current;
					friend class Map;
			};
			class ConstIterator{
			public:
				ConstIterator(const ConstIterator&);
				ConstIterator& operator=(const ConstIterator&);
				ConstIterator(ConstIterator&&);
				ConstIterator& operator=(ConstIterator&&);
				~ConstIterator();
				ConstIterator(const Iterator&);
				ConstIterator& operator++();
				ConstIterator operator++(int);
				ConstIterator& operator--();
				ConstIterator operator--(int);
				const std::pair<const Key, Value>& operator*() const;

				bool operator==(const ConstIterator& iter);
				bool operator!=(const ConstIterator& iter);
			private:
				Node* current;
				friend class Map;
			};
			class ReverseIterator{
			public:
				ReverseIterator(const ReverseIterator&);
				ReverseIterator& operator=(const ReverseIterator&);
				ReverseIterator(ReverseIterator&&);
				ReverseIterator& operator=(ReverseIterator&&);
				~ReverseIterator();
				ReverseIterator& operator++();
				ReverseIterator operator++(int);
				ReverseIterator& operator--();
				ReverseIterator operator--(int);
				const std::pair<const Key, Value>& operator*() const;

				bool operator==(const ReverseIterator& iter);
				bool operator!=(const ReverseIterator& iter);
			private:
				Node* current;
				friend class Map;
			};

			/*---Constructors---*/
			Map(); //Default Constructor
			Map(const Map&); //Copy Constructor
			Map& operator=(const Map&); //Copy Assignment
			Map(Map&&); //Move Constructor
			Map& operator=(Map&&); //Move Assignment
			Map(std::initializer_list<std::pair<const Key, Value>>);//Initializer List Constructor
			~Map();//Destructor

			/*---Modifiers---*/
			Iterator insert(const std::pair<const Key, Value>&); //Inserts given key, value pair
			Iterator insert(std::pair<const Key, Value>&&); //Inserts by moving instead of copying
			void erase(Iterator); //Removes element pointed to by the Iterator
			void remove(const Key&); //Removes element with the provided key
			void clear(); //Removes all elements from the map

			/*---Lookup---*/
			Iterator find(const Key&); //Returns an iterator to the element with the specified key
			ConstIterator find(const Key&) const; //Returns a const iterator to the element with the specified key
			Value& at(const Key&) const; //Returns a reference to the value at the specified key
			Value& operator[](const Key&);

			/*---Size---*/
			std::size_t size() const; //Returns the number of elements in the map
			bool empty() const; //Determines if the map is empty

			/*---Comparison Operators---*/
			bool operator==(const Map&) const;
			bool operator!=(const Map&) const;

			/*---Iterators---*/
			Iterator begin();
			Iterator end();
			ConstIterator begin() const;
			ConstIterator end() const;
			ReverseIterator rbegin();
			ReverseIterator rend();

		private:
			Iterator insert(const std::pair<const Key, Value>&, Node*);
			Iterator insert(std::pair<const Key, Value>&&, Node*);
			Iterator find(const Key&, Node*);
			void insertDLL(Node* pos, Node* n);
			void clear(Node*);
			void replace(Node*, Node*);
	};
}

#endif

/*Default Constructor
 *********************
 *Sets the default values
 *Head is set to null
 *Tail is set to a new empty Node
 */
template <typename Key, typename Value>
cs540::Map<Key, Value>::Map() : head(nullptr), tail(new Map<Key, Value>::Node()), m_size(0){
	tail->prev = tail;
	tail->next = tail;
}

/*Copy Constructor
 ******************
 *Makes a copy of the existing instance passed to it
*/
template <typename Key, typename Value>
cs540::Map<Key, Value>::Map(const Map& map2) : Map(){
	//Create a new Map
	//Finds all of the nodes in it and creates copies of them
	//	Calls the node copy constructor, which should maintain all Node* ptrs
	//Why is this not implemented????
	for(auto& i : map2) insert(i);
}

/*Copy Assignment
 *****************
 *Creates a deep copy using the overloaded assignment operator
 */
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Map& cs540::Map<Key,Value>::operator=(const Map& map2){
	//Calls the copy constructor for Map
}

/*Move Constructor
 *Creates a new instance of the passed in object but instead of copying it moves the resources 
 *	from the old object to the new object
 */
template <typename Key, typename Value>
cs540::Map<Key, Value>::Map(Map&& map2){

}

//Move Assignment
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Map& cs540::Map<Key, Value>::operator=(Map&& map2){
	//Calls the move constructor
}

//Initializer List Constructor
template <typename Key, typename Value>
cs540::Map<Key, Value>::Map(std::initializer_list<std::pair<const Key, Value>> n_value_in){
	head = nullptr;
	tail = new Map<Key, Value>::Node();
	for(auto i : n_value_in) insert(i);
}

//Destructor
template <typename Key, typename Value>
cs540::Map<Key, Value>::~Map(){
	if(head) clear(head);
	delete tail;
}

//Insert
//	Returns an iterator to the inserted node
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::insert(std::pair<const Key, Value>&& n_value_in){
	if(head){
		return this->insert(n_value_in, head);
	}
	else{
		head = new Node(n_value_in, nullptr);
		head->next = tail;
		tail->prev = head;
		m_size++;
	}
	return Map<Key, Value>::Iterator(head);
}

//Constant Insert
//	Returns an iterator to the inserted node
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::insert(const std::pair<const Key, Value>& n_value_in){
	if(head){
		return this->insert(n_value_in, head);
	}
	else{
		head = new Node(n_value_in, nullptr);
		head->next = tail;
		tail->prev = head;
		m_size++;
	}
	return Map<Key, Value>::Iterator(head);
}

int main{
	return 0;
}