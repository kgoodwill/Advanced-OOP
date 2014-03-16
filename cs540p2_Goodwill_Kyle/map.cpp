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
					Node *getBefore();
					Node *getAfter();
					~Node();
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
			Value& at(const Key&);
			const Value& at(const Key&) const; //Returns a reference to the value at the specified key
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
			ConstIterator find(const Key&, Node*) const;
			void insert_CLL(Node* pos, Node* n);
			void clear(Node*);
			void replace(Node*, Node*);
	};
}

#endif

/*Public Functions for Map Class*/

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
	//for(auto& i : map2) insert(i);
	auto i = map2.begin();
	auto j = map2.end();
	while(i != j){
		insert(*i);
		i++;
		if(i != j){
			j--;
			insert(*j);
		}
	}
	if(i != map2.end()){
		insert(*i);
	}
}

/*Copy Assignment
 *****************
 *Creates a deep copy using the overloaded assignment operator
 */
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Map& cs540::Map<Key,Value>::operator=(const Map& map2){
	if(*this == map2){
		return *this;
	}
	this->clear();
	auto i = map2.begin();
	auto j = map2.end();
	while(i != j){
		insert(*i);
		i++;
		if(i != j){
			j--;
			insert(*j);
		}
	}
	if(i != map2.end()){
		insert(*i);
	}
	return *this;
}

/*Move Constructor
 *Creates a new instance of the passed in object but instead of copying it moves the resources 
 *	from the old object to the new object
 */
template <typename Key, typename Value>
cs540::Map<Key, Value>::Map(Map&& map2){
	head = map2.head;
	tail = map2.tail;
	m_size = map2.m_size;
	map2.root = nullptr;
	map2.tail = nullptr;
	map2.m_size = 0;
}

//Move Assignment
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Map& cs540::Map<Key, Value>::operator=(Map&& map2){
	if(*this == *map2){
		return *this;
	}
	this->clear();
	this->m_size = map2.size();
	(this->head) = map2.head;
	(this->tail) = map2.tail;
	map2.head = nullptr;
	map2.tail = nullptr;
	map2.m_size = 0;
	return *this;
}

//Initializer List Constructor
template <typename Key, typename Value>
cs540::Map<Key, Value>::Map(std::initializer_list<std::pair<const Key, Value>> n_value_in){
	for(auto i : n_value_in) insert(i);
}

//Destructor
template <typename Key, typename Value>
cs540::Map<Key, Value>::~Map(){
	clear();
	if(tail) delete tail;
}

//Insert
//	Returns an iterator to the inserted node
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::insert(std::pair<const Key, Value>&& n_value_in){
	if(head){
		Node* newNode = new Node(n_value_in, nullptr);
		return this->insert(head, n_value_in);
	}
	else{
		head = new Node(n_value_in, nullptr);
		head->next = tail;
		head->prev = tail;
		tail->next = head;
		tail->prev = head;
		m_size++;
		return Map<Key, Value>::Iterator(head);
	}
}

//Constant Insert
//	Returns an iterator to the inserted node
template <typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::insert(const std::pair<const Key, Value>& n_value_in){
	if(head){
		Node* newNode = new Node(n_value_in, nullptr);
		return this->insert(head, newNode);
	}
	else{
		head = new Node(n_value_in, nullptr);
		head->next = tail;
		head->prev = tail;
		tail->prev = head;
		tail->next = head;
		m_size++;
		return Map<Key, Value>::Iterator(head);
	}
}

template<typename Key, typename Value>
void cs540::Map<Key, Value>::erase(Map<Key, Value>::Iterator iter){
	//Erases the data in the map at location specified by the iterator
	//Remove node from the LL first
	if(iter.current->prev){
		iter.current->prev->next = iter.current->next;
		iter.current->next->prev = iter.current->prev;
	}
	//Then remove node from the BST
	if(!iter.current->left){
		this->replace(iter.current, iter.current->right);
	}
	else if(!iter.current->right){
		this->replace(iter.current, iter->current->left);
	}
	else{
		Node* node = iter.current->right;
		while(node->left){
			node = node->left;
		}
		if(node->parent != iter.current){
			this->replace(node, node->right);
			node->right = iter.current->right;
			node->right->parent = node;
		}
		this->replace(iter.current, node);
		node->left = iter.current->left;
		node->left->parent = node;
	}
	delete iter.current;
}

template<typename Key, typename Value>
void cs540::Map<Key, Value>::remove(const Key& removeKey){
	//Removes the data in the map associated with the given key

	Map<Key, Value>::Iterator iter = this->find(removeKey);
	if(iter == this->end()){
		throw std::out_of_range("Key does not exist in this Map");
	}
	else{
		erase(iter);
	}
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::find(const Key& searchKey){
	//Returns an iterator to the location matching the search key
	if(!head){
		return Map<Key, Value>::Iterator(tail);
	}
	else{
		return find(searchKey, head);
	}
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator cs540::Map<Key, Value>::find(const Key& searchKey) const{
	//Does the same thing as regular iterator find but with a const iterator
	if(!head){
		return Map<Key, Value>::ConstIterator(tail);
	}
	else{
		return find(searchKey, head);
	}
}

template<typename Key, typename Value>
void cs540::Map<Key, Value>::clear(){
	//Resets all values of the map to default values
	if(head){
		clear(head);
		head = nullptr;
		tail->next = tail;
		tail->prev = tail;
		m_size = 0;
	}
}

template<typename Key, typename Value>
Value& cs540::Map<Key, Value>::at(const Key& searchKey){
	//Returns a pointer to the value at the specified search key
	auto iter = find(searchKey);
	if(iter.current	== tail){
		throw std::out_of_range("Key does not exist in this Map");
	}
	else{
		return (*iter).second;
	}
}

template<typename Key, typename Value>
const Value& cs540::Map<Key, Value>::at(const Key& searchKey) const{
	//Does the same thing as the non-const version
	auto iter = find(searchKey);
	if(iter.current == tail){
		throw std::out_of_range("Key does not exist in this Map");
	}
	else{
		return (*iter).second;
	}
}

template<typename Key, typename Value>
Value& cs540::Map<Key, Value>::operator[](const Key& key){
	//Returns an iterator to the specified key
	auto iter = insert({key, {}});
	return (*iter).second;
}

template<typename Key, typename Value>
std::size_t cs540::Map<Key, Value>::size() const{
	//Return the size of the map
	return m_size;
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::empty() const{
	//Return true if the map is empty
	return (m_size == 0);
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::operator==(const Map& map2) const{
	//Return true if the 2 maps are the same
	if(this->m_size == map2.m_size){
		auto i = this->begin();
		for(auto j = map2.begin(); j != map2.end(); j++){
			if(*i == *j){
				i++;
			}
			else{
				return false;
			}
		}
		return true;
	}
	else return false;
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::operator!=(const Map& map2) const{
	//Use the == operator and return the negative of that
	return !(this == map2);
}

//Iterators
template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::begin(){
	//Return an iterator to the beginning of the map
	return Map<Key, Value>::Iterator(tail->next);
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::end(){
	//Return an iterator to the end of the map
	return Map<Key, Value>::Iterator(tail);
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator cs540::Map<Key, Value>::begin() const{
	//Same as regular begin but const
	return Map<Key, Value>::ConstIterator(tail->next);
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator cs540::Map<Key, Value>::end() const{
	//Same as regular end but const
	return Map<Key, Value>::ConstIterator(tail);
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ReverseIterator cs540::Map<Key, Value>::rbegin(){
	//Returns an iterator to the beginning of the map, going in the opposite direction
	return Map<Key, Value>::ReverseIterator(tail->prev);
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ReverseIterator cs540::Map<Key, Value>::rend(){
	//Returns an iterator to the end of the map
	return Map<Key, Value>::ReverseIterator(tail);
}

/*Private Functions for Map Class*/
template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::insert(Node* node, Node* nodeIn){
	if(node->key() < nodeIn->key()){
		if(!node->right){
			m_size++;
			node->right = nodeIn;
			nodeIn->parent = node;
			insert_CLL(nodeIn->get_pred(), nodeIn);
			return Map<Key, Value>::Iterator(nodeIn);
		}
		else{
			return this->insert(node->right, nodeIn);
		}
	}
	else if(node->key() > nodeIn->key()){
		if(!node->left){
			m_size++;
			node->left = nodeIn;
			nodeIn->parent = node;
			insert_CLL(nodeIn->get_pred(), nodeIn);
			return Map<Key, Value>::Iterator(nodeIn);
		}
		//Same key
		else{
			return this->insert(node->left, nodeIn);
		}
	}
	else{
		delete nodeIn;
		return Map<Key, Value>::Iterator(node);
	}
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::find(const Key& searchKey, Node* node){
	if(node == nullptr){
		return Map<Key, Value>::Iterator(tail);
	}
	else if (node->key() == searchKey){
		return Map<Key, Value>::Iterator(node);
	}
	else if (searchKey > node->key()){
		return find(searchKey, node->right);
	}
	else{
		return find(searchKey, node->left);
	}
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator cs540::Map<Key, Value>::find(const Key& searchKey, Node* node) const{
	if(node == nullptr){
		return Map<Key, Value>::ConstIterator(tail);
	}
	else if (node->key() == searchKey){
		return Map<Key, Value>::ConstIterator(node);
	}
	else if (searchKey > node->key()){
		return find(searchKey, node->right);
	}
	else{
		return find(searchKey, node->left);
	}
}

template<typename Key, typename Value>
void cs540::Map<Key, Value>::insert_CLL(Node* position, Node* nodeIn){
	if(position){
		nodeIn->prev = position;
		nodeIn->next = position->next;
		position->next->prev = nodeIn;
		position->next = nodeIn;
	}
	else{
		nodeIn->prev = tail;
		nodeIn->next = tail->next;
		tail->next->prev = nodeIn;
		tail->next = nodeIn;
	}
}

template<typename Key, typename Value>
void cs540::Map<Key, Value>::clear(Node* node){
	if(node){
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

template<typename Key, typename Value>
void cs540::Map<Key, Value>::replace(Node* nodeA, Node* nodeB){
	if(!nodeA->parent){
		head = nodeB;
	}
	else if (nodeA->parent->left && nodeA->parent->left == nodeA){
		nodeA->parent->left = nodeB;
	}
	else{
		nodeA->parent->right = nodeB;
	}
	if(nodeB){
		nodeB->parent = nodeA->parent;
	}
}

/*Iterator, ConstIterator & ReverseIterator Functions*/
//Iterator
template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator& cs540::Map<Key, Value>::Iterator::operator++(){
	this->current = this->current->next;
	return *this;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::Iterator::operator++(int){
	auto iter = *this;
	this->current = this->current->next;
	return iter;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator& cs540::Map<Key, Value>::Iterator::operator--(){
	this->current = this->current->prev;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Iterator cs540::Map<Key, Value>::Iterator::operator--(int){
	auto iter = *this;
	this->current = this->current->prev;
	return iter;
}

template<typename Key, typename Value>
std::pair<const Key, Value>& cs540::Map<Key, Value>::Iterator::operator*() const{
	return *(this->current->n_value);
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::Iterator::operator==(const Iterator& iter){
	return this->current == iter.current;
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::Iterator::operator!=(const Iterator& iter){
	return this->current != iter.current;
}

//ConstIterator
template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator& cs540::Map<Key, Value>::ConstIterator::operator++(){
	this->current = this->current->next;
	return *this;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator cs540::Map<Key, Value>::ConstIterator::operator++(int){
	auto iter = *this;
	this->current = this->current->next;
	return iter;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator& cs540::Map<Key, Value>::ConstIterator::operator--(){
	this->current = this->current->prev;
	return *this;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ConstIterator cs540::Map<Key, Value>::ConstIterator::operator--(int){
	auto iter = *this;
	this->current = this->current->prev;
	return iter;
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::ConstIterator::operator==(const ConstIterator& iter){
	return this->current == iter.current;
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::ConstIterator::operator!=(const ConstIterator& iter){
	return this->current != iter.current;
}

template<typename Key, typename Value>
const std::pair<const Key, Value>& cs540::Map<Key, Value>::ConstIterator::operator*() const{
	return *(this->current->n_value);
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ReverseIterator& cs540::Map<Key, Value>::ReverseIterator::operator++(){
	this->current = this->current->prev;
	return *this;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ReverseIterator cs540::Map<Key, Value>::ReverseIterator::operator++(int){
	auto iter = *this;
	this->current = this->current->prev;
	return iter;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ReverseIterator& cs540::Map<Key, Value>::ReverseIterator::operator--(){
	this->current = this->current->next;
	return *this;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::ReverseIterator cs540::Map<Key, Value>::ReverseIterator::operator--(int){
	auto iter = *this;
	this->current = this->current->next;
	return iter;
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::ReverseIterator::operator==(const ReverseIterator& iter){
	return this->current == iter.current;
}

template<typename Key, typename Value>
bool cs540::Map<Key, Value>::ReverseIterator::operator!=(const ReverseIterator& iter){
	return this->current != iter.current;
}

template<typename Key, typename Value>
const std::pair<const Key, Value>& cs540::Map<Key, Value>::ReverseIterator::operator*() const{
	return *(this->current->n_value);
}

//Node Functions
template<typename Key, typename Value>
cs540::Map<Key, Value>::Node::~Node(){
	if(n_value){
		delete n_value;
	}
}

template<typename Key, typename Value>
Key cs540::Map<Key, Value>::Node::key(){
	return n_value->first;
}

template<typename Key, typename Value>
Value cs540::Map<Key, Value>::Node::value(){
	return n_value->second;
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Node* cs540::Map<Key, Value>::Node::getBefore(){
	Node* returnNode;
	if(this->left){
		returnNode = this->left;
		while(returnNode->right){
			returnNode = returnNode->right;
		}
		return returnNode;
	}
	else{
		Node *currentNode = this;
		returnNode = this->parent;
		while(returnNode){
			if(returnNode->right && returnNode->right == currentNode){
				return returnNode;
			}
			currentNode = returnNode;
			returnNode = returnNode->parent;
		}
		return nullptr;
	}
}

template<typename Key, typename Value>
typename cs540::Map<Key, Value>::Node* cs540::Map<Key, Value>::Node::getAfter(){
	Node* returnNode;
	if(this->right){
		returnNode = this->right;
		while(returnNode->left){
			returnNode = returnNode->left;
		}
		return returnNode;
	}
	else{
		Node *currentNode = this;
		returnNode = this->parent;
		while(returnNode){
			if(returnNode->left && returnNode->left == currentNode){
				return returnNode;
			}
			currentNode = returnNode;
			returnNode = returnNode->parent;
		}
		return nullptr;
	}
}