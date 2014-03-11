//Map Class

#ifndef MAP_HPP
#define MAP_HPP

#include <stdio>
#include <stdlib>
#include <iostream>

namespace cs540{
	template <typename key, typename value>

	class Map{
		private:
			class Node{
				public:
					std::pair<const Key, Value> *n_value;
					Node* left; //BST Left
					Node* right; //BST Right
					Node* parent; //BST Parent
					Node* prev; //LL Previous
					Node* next; //LL Next

					Node() : prev(nullptr), next(nullptr) {}
					Node(std::pair<const Key, Value> dataIn, Node* parentIn) : n_value(new std::pair<const Key, Value>(dataIn)), left(nullptr), right(nullptr), parent(nullptr), prev(nullptr), next(nullptr) {}
					Key key();
					Value value();
					Node *get_before();
					Node *get_after();
			};
			Node* head;
			Node* tail;
			std::size_t size;

		public:
			class Iterator{};
			class ConstIterator{};
			class ReverseIterator{};
			/*---Constructors---*/
			Map(); //Default Constructor
			Map(const &Map); //Copy Constructor
			Map& operator=(const &Map); //Copy Assignment
			Map(Map&&); //Move Constructor
			Map& operator=(Map&&); //Move Assignment
			Map(std::initializer_list<std::pair<const Key, Value>>);//Initializer List Constructor
			~Map();//Destructor

			/*---Modifiers---*/
			Iterator insert(const std::pair<const Key, Value>&); //Inserts given key, value pair
			Iterator insert(std::pair<const Key, value>&&); //Inserts by moving instead of copying
			void erase(Iterator); //Removes element pointed to by the Iterator
			void remove(const Key&); //Removes element with the provided key
			void clear(); //Removes all elements from the map

			/*---Lookup---*/
			Iterator find(const Key&); //Returns an iterator to the element with the specified key
			ConstIterator find(const Key&); //Returns a const iterator to the element with the specified key
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
			void insert_list(Node* pos, Node* n);
			void clear(Node*);
			void replace(Node*, Node*);

	};
}

#endif

