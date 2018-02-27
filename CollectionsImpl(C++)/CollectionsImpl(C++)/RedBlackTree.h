#pragma once
#include "stdafx.h"
#include <vector>
#include <stdexcept>
#include <iostream>

/*T is the data type of the contents the tree holds, while
U is the data type of what the tree uses for node keys.*/
template<typename T, typename U = double>
class RedBlackTree
{
	struct Node
	{
		const static int NIL = -1;
		int left_index = NIL;
		int right_index = NIL;
		int parent_index = NIL;
		T content;
		U key;
		int index;

		enum Color
		{
			red = 0,
			black = 1
		};
		Color color = Color::red;

		/*Node()
		{
			std::cout << "Constructing Node..." << std::endl;
		}*/
		Node() = default;
		//~Node() = default;
		Node(Node&& other) = default;

		/*~Node()
		{
			std::cout << "Deconstructing Node..." << std::endl;
		}*/
	};

	class NodeContainer
	{
		Node* first = nullptr;
		size_t size = 0;
		size_t capacity;

		/*Frees up dynamically allocated resources.Deconstructors are called
		  only on memory that has been constructed.*/
		void Release()
		{
			for (int i = 0;i < size;i++)
			{
				(first + i)->~Node();
			}
			delete(first);
		}

	public:

		NodeContainer(unsigned int cap)
		{
			Reserve(cap);
		}

		~NodeContainer()
		{
			Release();
			//std::cout << "Called" << std::endl;
		}

		/*Allocates memory for atmost param cap nodes.
		Note that when using a pointer's version of new(size_t),
		memory is allocated without constructing anything.*/
		void Reserve(unsigned int cap)
		{
			if (cap == 0)
			{
				throw new std::invalid_argument(
					"capacity must be greater than zero");
			}
			Node* temp = (Node*)::operator new(cap * sizeof(Node));
			if (first != nullptr)
			{
				for (int i = 0;i < size;i++)
				{
					new(temp + i) Node(std::move(*(first + i)));
				}
				Release();
			}
			first = &(*temp);
			capacity = cap;
		}

		/*Trims excessive capacity, but throws an exception
		  if the size is 0.*/
		void TrimExcess()
		{
			Reserve(size);
		}

		/*Adds a node to the tree.*/
		void Push(Node&& node)
		{
			if (size == capacity)
			{
				Reserve(capacity * 2);
			}	
			new(first + size) Node(std::move(node));
			size++;
		}

		Node& operator[](unsigned int position)
		{
			if (position >= size)
			{
				throw new std::out_of_range("");
				/*throw new std::out_of_range(
					"Position has to be less than size."
					+ "Position is " + position + " and"
					+ " size is " + size + ".");*/
			}
			return *(first + position);
		}

		size_t GetSize()
		{
			return size;
		}

		void Clear()
		{
			if (size > 0)
			{
				for (int i = size - 1;i >= 0;i--)
				{
					(first + i)->~Node();
				}
				size = 0;
			}
		}
	};

	//std::vector<Node> nodes;
	NodeContainer nodes;
	int root_index;
	/*Places the node based on the key the node has
	been given.*/
	void InitiallyPlace(int index)
	{
		//std::cout << "Placing..." << std::endl;
		Node& node = nodes[index];
		if (nodes.GetSize() == 1)
		{
			root_index = index;
		}
		else
		{
			int place = root_index;
			int prev_place = place;
			while (place != Node::NIL)
			{
				prev_place = place;
				/*If the node being placed has a key less than or equal to
				the root of the current subtree's root node,search the
				left inner subtree.*/
				if (node.key <= nodes[place].key)
				{
					place = nodes[place].left_index;
				}
				/*Else search the right inner subtree,both until you reach the
				end (null node)*/
				else if (node.key > nodes[place].key)
				{
					place = nodes[place].right_index;
				}
			}
			if (node.key <= nodes[prev_place].key)
			{
				nodes[prev_place].left_index = index;
			}
			else
			{
				nodes[prev_place].right_index = index;
			}
			node.parent_index = prev_place;
		}
	}

	/*Gets the sibling index of the node.*/
	int GetNodeSibling(int index)
	{
		if (index == root_index)
		{
			return Node::NIL;
		}
		int parent_index = nodes[index].parent_index;
		if (index == nodes[parent_index].left_index)
		{
			return nodes[parent_index].right_index;
		}
		else
		{
			return nodes[parent_index].left_index;
		}
	}

	/*Restructures the positions of the
	parent and grandparent nodes*/
	void Restructure(int index)
	{
		//std::cout << "Restructuring..." << std::endl;
		//std::cout << index << std::endl;
		Node* node = &nodes[index];
		//std::cout << node.parent_index << std::endl;
		switch (nodes[node->parent_index].color)
		{
		case Node::Color::black:
			break;
		case Node::Color::red:
			int sibling_index = GetNodeSibling(node->parent_index);
			if (sibling_index == Node::NIL
				|| nodes[sibling_index].color == Node::Color::black)
			{
				Rotate(index);
				if (node->parent_index != Node::NIL)
				{
					//std::cout << node->key << " is now a child of " << nodes[node->parent_index].key << ", " << std::endl;
					if (sibling_index != Node::NIL)
					{
						//std::cout << "With " << nodes[sibling_index].key << " as a sibling" << std::endl;
					}
					if (nodes[node->parent_index].parent_index != Node::NIL)
					{
						//std::cout << "With " << nodes[node->parent_index].key << " as a grandparent" << std::endl;
					}
				}
				break;
			}
			/*Note,when the color of the sibling is red,
			The switch statement is continued to ensure
			that recolorings that cause a parent and
			child to both be red are dealt with.
			The node is set to the grandparent, for
			the grandparent and parent cases are already
			resolved.*/
			else if (nodes[sibling_index].color == Node::Color::red)
			{
				Recolor(index);
				int grand_parent_index = nodes[node->parent_index].parent_index;
				node = &nodes[grand_parent_index];
				index = grand_parent_index;
			}
		}
	}


	/*Rotates the parent and the grandparent of the
	node.*/
	void Rotate(int index)
	{
		//std::cout << "Rotating..." << std::endl;
		int parent_index = nodes[index].parent_index;
		int grand_parent_index = nodes[parent_index].parent_index;
		//std::cout << grand_parent_index << std::endl;
		Node& node = nodes[index];
		Node& parent = nodes[parent_index];
		Node& grand_parent = nodes[grand_parent_index];
		int root_candidate = root_index;
		/*If the parent of the node is in the left subtree of the
		  grandparent's...*/
		if (parent_index == grand_parent.left_index)
		{
			/*If the node is in the left subtree of the its
			 parent,shift the nodes right.*/
			if (index == parent.left_index)
			{
				/*Make the parent a parent of the grandparent,
				with the grandparent becoming the right subtree
				of the parent.*/
				parent.parent_index = grand_parent.parent_index;
				if (grand_parent.parent_index != Node::NIL)
				{
					if (nodes[grand_parent.parent_index].key >= parent.key)
					{
						nodes[grand_parent.parent_index].left_index = parent_index;
					}
					else
					{
						nodes[grand_parent.parent_index].right_index = parent_index;
					}
				}
				grand_parent.parent_index = parent_index;
				/*Make the previous right subtree of the parent
				Equivalent to the new grandparents left
				subtree.*/
				grand_parent.left_index = parent.right_index;
				parent.right_index = grand_parent_index;
	
			
				parent.color = Node::Color::black;
				grand_parent.color = Node::Color::red;
				root_candidate = parent_index;
			}
			/*If the node is in the right parent subtree,
			  Swap it and the grandparent's places.*/
			else
			{
				/*Make the node a parent of both the grandparent
				and the parent. */
				node.parent_index = grand_parent.parent_index;
				if (grand_parent.parent_index != Node::NIL)
				{
					if (nodes[grand_parent.parent_index].key >= node.key)
					{
						nodes[grand_parent.parent_index].left_index = index;
					}
					else
					{
						nodes[grand_parent.parent_index].right_index = index;
					}
				}
				grand_parent.parent_index = index;
				parent.parent_index = index;
				/*Set the newly created left subtree of the grandparent
				 and right subtree of the parent as the node's 
				 right subtree and left subtree respectively.*/
				grand_parent.left_index = node.right_index;
				parent.right_index = node.left_index;
				/*Make the parent its left subtree and the
				grandparent its right subtree.*/
				node.left_index = parent_index;
				node.right_index = grand_parent_index;

				node.color = Node::Color::black;
				grand_parent.color = Node::Color::red;
				root_candidate = index;
			}
		}
		/*If the parent of the node is in the right subtree of the
		grandparent's...*/
		else if(parent_index == grand_parent.right_index)
		{
			/*If the node is in the right subtree of the its
			parent,shift the nodes left.*/
			if /*(node.key > parent.key)*/ (index == parent.right_index)
			{
				/*Make the parent a parent of the grandparent,
				with the grandparent becoming the left subtree
				of the parent.*/
				parent.parent_index = grand_parent.parent_index;
				if (grand_parent.parent_index != Node::NIL)
				{
					if (nodes[grand_parent.parent_index].key >= parent.key)
					{
						nodes[grand_parent.parent_index].left_index = parent_index;
					}
					else
					{
						nodes[grand_parent.parent_index].right_index = parent_index;
					}
				}
				grand_parent.parent_index = parent_index;
				/*Make the previous left subtree of the parent
				Equivalent to the new grandparents right
				subtree.*/
				grand_parent.right_index = parent.left_index;
				parent.left_index = grand_parent_index;

				parent.color = Node::Color::black;
				grand_parent.color = Node::Color::red;
				root_candidate = parent_index;
			}
			/*If the node is in the left parent subtree,
			Swap it and the grandparent's places.*/
			else
			{
				/*Make the node a parent of both the grandparent
				and the parent. */
				node.parent_index = grand_parent.parent_index;
				if (grand_parent.parent_index != Node::NIL)
				{
					if (nodes[grand_parent.parent_index].key >= node.key)
					{
						nodes[grand_parent.parent_index].left_index = index;
					}
					else
					{
						nodes[grand_parent.parent_index].right_index = index;
					}
				}
				grand_parent.parent_index = index;
				parent.parent_index = index;
				/*Set the newly created right subtree of the grandparent
				and left subtree of the parent as empty.*/
				grand_parent.right_index = node.left_index;
				parent.left_index = node.right_index;
				/*Make the parent its right subtree and the
				grandparent its left subtree.*/
				node.right_index = parent_index;
				node.left_index = grand_parent_index;

				node.color = Node::Color::black;
				grand_parent.color = Node::Color::red;
				root_candidate = index;
			}
		}
		if (root_index == grand_parent_index)
		{
			root_index = root_candidate;
		}
	}

	/*Performs recoloring of the node,its parent,
	and its grandparent.*/
	void Recolor(int index)
	{
		//std::cout << "Recoloring..." << std::endl;
		int parent = nodes[index].parent_index;
		int uncle = GetNodeSibling(parent);
		int grand_parent = nodes[parent].parent_index;
		SwitchColor(uncle);
		SwitchColor(parent);
		SwitchColor(grand_parent);
	}

	/*Switches the color of the node.*/
	void SwitchColor(int index)
	{
		Node& node = nodes[index];
		if (node.color == Node::Color::black && index != root_index)
		{
			node.color = Node::Color::red;
		}
		else
		{
			node.color = Node::Color::black;
		}
	}

	/*Returns the node with the smallest key.*/
	Node& Minimum()
	{
		Node* node = &nodes[root_index];
		while (node->left_index != Node::NIL)
		{
			node = &nodes[node->left_index];
		}
		return *node;
	}

	/*Returns the node with the largest key.*/
	Node& Maximum()
	{
		Node* node = &nodes[root_index];
		while (node->right_index != Node::NIL)
		{
			node = &nodes[node->right_index];
		}
		return *node;
	}

public:
	class RBIterator : public std::iterator<
		std::random_access_iterator_tag,
		T,
		std::ptrdiff_t,
		Node*,
		T&>
	{
		Node* ptr = nullptr;
		RedBlackTree* tree;
	public:
		typedef RBIterator iterator;
		iterator(Node* other,RedBlackTree* _tree)
		{
			ptr = other;
			tree = _tree;
		}
		iterator(const iterator& other) = default;
		iterator(iterator&& other) = default;
		~RBIterator() = default;

		iterator& operator++()
		{
			int last = tree->nodes.GetSize() - 1;
			if (ptr != &tree->nodes[last] + 1)
			{
				ptr++;
			}
			return *this;
		}

		iterator operator++(ptrdiff_t num)
		{
			int last = tree->nodes.GetSize() - 1;
			if (ptr != &tree->nodes[last] + 1)
			{
				ptr++;
			}
			return iterator(ptr - 1,tree);
		}

		iterator& operator--()
		{
			ptr--;
			return *this;
		}

		iterator operator--(ptrdiff_t num)
		{
			ptr--;
			return iterator(ptr + 1,tree);
		}

		iterator& operator+=(ptrdiff_t& num)
		{
			int last = tree->nodes.GetSize() - 1;
			if (ptr != &tree->nodes[last] + 1)
			{
				ptr += num;
			}
			return *this;
		}

		iterator& operator-=(ptrdiff_t& num)
		{
			ptr -= num;
			return *this;
		}

		iterator& operator+(ptrdiff_t& num)
		{
			return iterator(ptr + num,tree);
		}

		iterator& operator-(ptrdiff_t& num)
		{
			return iterator(ptr - num,tree);
		}

		T& operator*()
		{
			return ptr->content;
		}

		bool operator==(const iterator& other)
		{
			return ptr == other.ptr;
		}

		bool operator!=(const iterator& other)
		{
			return !(*this == other);
		}

		bool operator<(const iterator& rhs)
		{
			return this->ptr < rhs.ptr;
		}

		bool operator>(const iterator& rhs)
		{
			return this->ptr > rhs.ptr;
		}

		bool operator<=(const iterator& rhs)
		{
			return this->ptr <= rhs.ptr;
		}

		bool operator>=(const iterator& rhs)
		{
			return this->ptr >= rhs.ptr;
		}
	};

	typedef RBIterator iterator;
	RedBlackTree(unsigned int capacity = 8) : nodes(capacity) {}

	size_t GetSize()
	{
		return nodes.GetSize();
	}
	/*Returns the element that has the specified
	key.*/
	T Search(U _key)
	{
		if (nodes.GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		Node* node;
		int index = root_index;
		while (index != Node::NIL)
		{
			node = &nodes[index];
			if (_key = node->key)
			{
				return node->content;
			}
			else if (_key <= node->key)
			{
				index = node->left_index;
			}
			else
			{
				index = node->right_index;
			}
		}
		throw std::invalid_argument("There is no object with the key " + _key + " in this container.");
	}

	/*Returns the smallest key in the container.*/
	U MinimumKey()
	{
		if (nodes.GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Minimum().key;
	}

	/*Returns the largest key in the container.*/
	U MaximumKey()
	{
		if (nodes.GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Maximum().key;
	}

	/*Returns the object with smallest key in the container.*/
	T MinimumVal()
	{
		if (nodes.GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Minimum().content;
	}

	/*Returns the object with largest key in the container.*/
	T MaximumVal()
	{
		if (nodes.GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Maximum().content;
	}

	/*Inserts the element into the tree.*/
	void Insert(T element, U _key)
	{
		/*Create and place the node into the tree.*/
		Node node;
		node.content = element;
		node.color = Node::Color::red;
		node.key = _key;
		node.index = nodes.GetSize();
		int index = node.index;
		if (nodes.GetSize() == 0)
		{
			node.color = Node::Color::black;
		}
		nodes.Push(std::move(node));
		InitiallyPlace(index);
		/*Restructure the node to make sure that
		there's no red node that parents another red
		node.*/
		if (index != root_index)
		{
			Restructure(index);
		}
	}

	void Clear()
	{
		nodes.Clear();
		root_index = Node::NIL;
	}

	void TrimExcess()
	{
		nodes.TrimExcess();
	}

	void Reserve(unsigned int capacity)
	{
		nodes.Reserve(capacity);
	}

	RBIterator Begin()
	{
		return RBIterator(&nodes[0],this);
	}

	const RBIterator CBegin()
	{
		return const Begin();
	}

	RBIterator End()
	{
		int last = nodes.GetSize() - 1;
		return RBIterator(&nodes[last] + 1,this);
	}

	const RBIterator CEnd()
	{
		return const End();
	}

	friend std::ostream& operator<<(std::ostream& stream, RedBlackTree tree)
	{
		/*Node& root = tree.nodes[tree.root_index];
		for (int i = 0;i < tree.nodes.size();i++)
		{
			stream << tree.nodes[i].content << " ";
		}
		stream << std::endl;
		return stream;*/
	}

};


