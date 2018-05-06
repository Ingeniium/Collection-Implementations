#pragma once
#include "stdafx.h"
#include <unordered_set>
#include <stdexcept>
#include <iostream>
#include <tuple>
#include <algorithm>

/*T is the data type of the contents the tree holds, while
U is the data type of what the tree uses for node keys.*/
template<typename T, typename U = double>
class RedBlackTree
{
	struct Node
	{
		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent = nullptr;
		T content;
		U key;

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
		~Node()
		{
			if (left != nullptr)
			{
				delete left;
			}
			if (right != nullptr)
			{
				delete right;
			}
		}

		Node(Node&& other) = default;

		bool SetParent(Node* new_parent)
		{
			if (parent != nullptr)
			{
				parent->RemoveChild(this);
			}
			if (new_parent != nullptr)
			{
				new_parent->SetChild(this);
			}
			else
			{
				parent = nullptr;
			}
			return parent == nullptr;
		}

		void SetChild(Node* node)
		{
			if (node == nullptr)
			{
				return;
			}
			if (node->key < key)
			{
				left = node;
			}
			else
			{
				right = node;
			}
			if (node->parent != nullptr)
			{
				node->parent->RemoveChild(node);
			}
			node->parent = this;
		}

		void RemoveChild(Node* node)
		{
			if (node != nullptr)
			{
				//node->parent = nullptr;
				if (node == left)
				{
					left = nullptr;
					node->parent = nullptr;
				}
				else if (node == right)
				{
					right = nullptr;
					node->parent = nullptr;
				}
			}
		}

		bool HasOneChild()
		{
			return (left == nullptr) ^ (right == nullptr);
		}

		bool isLeaf()
		{
			return (left == nullptr && right == nullptr);
		}

		bool HasTwoChildren()
		{
			return (left != nullptr && right != nullptr);
		}

		Node* GetLoneChild()
		{
			/*if (isLeaf())
			{
				throw std::logic_error("Node has no children!");
			}*/
			if (right != nullptr)
			{
				return right;
			}
			else if(left != nullptr)
			{
				return left;
			}
			else
			{
				return nullptr;
			}
		}

		/*~Node()
		{
			std::cout << "Deconstructing Node..." << std::endl;
		}*/
	};

	Node* root = nullptr;
	int size = 0;
	Node** sorted_pointers = nullptr;
	/*Places the node based on the key the node has
	been given.*/
	void InitiallyPlace(Node* node)
	{
		//std::cout << "Placing..." << std::endl;	
		if (GetSize() == 1)
		{
			root = node;
		}
		else
		{
			Node* current = root;
			Node* prev = current;
			while (current != nullptr)
			{
				prev = current;
				/*If the node being placed has a key less than or equal to
				the root of the current subtree's root node,search the
				left inner subtree.*/
				if (node->key < current->key)
				{
					current = current->left;
				}
				else if (node->key == current->key)
				{
					size--;
					delete node;
					node = nullptr;
					return;
				}
				/*Else search the right inner subtree,both until you reach the
				end (null node)*/
				else if (node->key > current->key)
				{
					current = current->right;
				}
			}
			prev->SetChild(node);
		}
	}

	struct RemoveGroup
	{
		Node* node;
		Node* sibling;
		Node* parent;
		typename Node::Color color;
		RemoveGroup(Node* _node, Node* _sibling,Node* _parent, typename Node::Color _color)
		{
			node = _node;
			sibling = _sibling;
			parent = _parent;
			color = _color;
		}
	};

	///*Standard binary search tree remove that's done before
	//RBD modifications.*/
	RemoveGroup InitiallyDelete(Node* node)
	{
		typename Node::Color deleted_color = node->color;
		size--;
		/*If node has one child,simply replace that node
		with its child.*/
		if (node->isLeaf())
		{
			Node* parent = node->parent;
			if(node->parent != nullptr)
			node->parent->RemoveChild(node);
			delete node;
			Node* sibling = (parent == nullptr || parent->isLeaf()) 
				? nullptr : parent->GetLoneChild();
			return RemoveGroup(nullptr, sibling,parent, deleted_color);
		}
		else if (node->HasOneChild())
		{
			Node* parent = node->parent;
			Node* child = node->GetLoneChild();
			if (parent != nullptr)
			{
				parent->RemoveChild(node);
				child->SetParent(parent);
			}
			else
			{
				root = child;
				child->parent = nullptr;
			}
			node->left = nullptr;
			node->right = nullptr;
			delete node;
			return RemoveGroup(child, GetNodeSibling(child),parent, deleted_color);
		}
		/*If node has two children,replace it with its
		in order succesor.*/
		else if (node->HasTwoChildren())
		{
			Node* successor = InOrderSuccessor(node);
			node->key = successor->key;
			node->content = successor->content;
			return InitiallyDelete(successor);
		}
	}

	void RestructureAfterDeletion(Node* node,Node* sibling,Node* parent, typename Node::Color deleted_color)
	{
		if (deleted_color == Node::Color::red || (node != nullptr && node->color == Node::Color::red))
		{
			if(node != nullptr)
			{
				node->color = Node::Color::black;
			}
		}
		else
		{
			typename Node::Color color = sibling == nullptr
				? Node::Color::black : sibling->color;
			switch (color)
			{
				case Node::Color::black:
				{
					if (sibling != nullptr && ((sibling->left != nullptr
						&& sibling->left->color == Node::Color::red)
						|| (sibling->right != nullptr
							&& sibling->right->color == Node::Color::red)))
					{
						Node* child = (sibling->right != nullptr
							&& sibling->right->color == Node::Color::red)
							? sibling->right : sibling->left;
						Rotate(child);
						break;
					}
					else
					{
						if (sibling != nullptr)
						{
							sibling->color = Node::Color::red;
						}
						node = parent;
						if (node == nullptr)
						{
							break;
						}
						parent = node->parent != nullptr ? node->parent : parent;
						if (node->color == Node::Color::red || node == root)
						{
							node->color = Node::Color::black;
							break;
						}
						sibling = GetNodeSibling(node);
					}
				}
				case Node::Color::red:
				{	
					parent->RemoveChild(sibling);
					if (parent->parent != nullptr)
					{
						parent->parent->SetChild(sibling);
					}
					if (sibling->key > parent->key)
					{
						FillLeftSubtree(sibling, parent);
					}
					else
					{
						FillRightSubtree(sibling, parent);
					}
				    if (parent == root)
					{
						root = sibling;
					}
					sibling->color = Node::Color::black;
					color = sibling->color;
					parent->color = Node::Color::red;
					sibling = node == nullptr ? parent->GetLoneChild()
						: GetNodeSibling(node);
				}
			}
		}
	}


	Node* FindNode(const U& _key)
	{
		Node* node = root;
		while (node != nullptr)
		{
			if (_key == node->key)
			{
				return node;
			}
			else if (_key < node->key)
			{
				node = node->left;
			}
			else
			{
				node = node->right;
			}
		}
		return nullptr;
	}

	/*Gets the sibling of the node.*/
	Node* GetNodeSibling(Node* node)
	{
		if (node == root)
		{
			return nullptr;
		}
		Node* parent = node->parent;
		if (parent->left == node)
		{
			return parent->right;
		}
		else
		{
			return parent->left;
		}
	}

	/*Restructures the positions of the
	parent and grandparent nodes*/
	void RestructureAfterInsert(Node* node)
	{
		//std::cout << "Restructuring..." << std::endl;
		Node* parent = node->parent;
		typename Node::Color color = parent->color;
		while (true)
		{
			switch (color)
			{
			case Node::Color::black:
				return;
			case Node::Color::red:
				Node* uncle = GetNodeSibling(parent);
				if (uncle == nullptr
					|| uncle->color == Node::Color::black)
				{
					Rotate(node);
					//violation(root);
					return;
				}
				/*Note,when the color of the sibling is red,
				The switch statement is continued to ensure
				that recolorings that cause a parent and
				child to both be red are dealt with.
				The node is set to the grandparent, for
				the grandparent and parent cases are already
				resolved.*/
				else if (uncle->color == Node::Color::red)
				{
					Recolor(node);
					Node* grand_parent = parent->parent;
					node = grand_parent;
					parent = node->parent;
					color = parent == nullptr ?
						Node::Color::black : parent->color;					
					break;
				}
			}
		}
	}

	void violation(Node* node)
	{
		if (node == nullptr)
		{
			return;
		}
		if (node->parent != nullptr && node->parent->color == Node::Color::red
			&& node->color == Node::Color::red)
		{
			std::cout << "Violation!" << std::endl;
		}
		violation(node->left);
		violation(node->right);
	}

	void Shift(Node* node,Node* _parent = nullptr,Node* _grand_parent = nullptr)
	{
		Node* parent = _parent == nullptr 
			? node->parent : _parent;
		Node* grand_parent = _grand_parent == nullptr 
			? parent->parent : _grand_parent;
		Node* great_parent = grand_parent->parent;
		parent->SetParent(great_parent);
		if (node->key <= parent->key)
		{
			FillRightSubtree(parent, grand_parent);
		}
		else
		{
			FillLeftSubtree(parent, grand_parent);
		}
	}

	void FillRightSubtree(Node* new_parent, Node* old_parent)
	{
		old_parent->SetChild(new_parent->right);
		new_parent->SetChild(old_parent);
	}

	void FillLeftSubtree(Node* new_parent, Node* old_parent)
	{
		old_parent->SetChild(new_parent->left);
		new_parent->SetChild(old_parent);
	}

	void SwapParenthood(Node* node)
	{
		Node* parent = node->parent;
		Node* grand_parent = parent->parent;
		if (node->key > parent->key)
		{
			parent->SetChild(node->left);
			grand_parent->SetChild(node->right);
		}
		else
		{
			parent->SetChild(node->right);
			grand_parent->SetChild(node->left);
		}
		node->SetParent(grand_parent->parent);
		parent->SetParent(node);
		grand_parent->SetParent(node);
	}

	/*Rotates the parent and the grandparent of the
	node.*/
	void Rotate(Node* node)
	{
		//std::cout << "Rotating..." << std::endl;
		Node* parent = node->parent;
		Node* grand_parent = parent->parent;
		Node* root_candidate = nullptr;
		if ((grand_parent->right == parent && parent->right == node)
			|| (grand_parent->left == parent && parent->left == node))
		{
			Shift(node);
			parent->color = Node::Color::black;
			grand_parent->color = Node::Color::red;
			root_candidate = parent;
		}
		else
		{
			SwapParenthood(node);
			node->color = Node::Color::black;
			grand_parent->color = Node::Color::red;
			root_candidate = node;
		}
		if (root == grand_parent && root_candidate != nullptr)
		{
			root = root_candidate;
		}
	}

	/*Performs recoloring of the node,its parent,
	and its grandparent.*/
	void Recolor(Node* node)
	{
		//std::cout << "Recoloring..." << std::endl;
		Node* parent = node->parent;
		Node* uncle = GetNodeSibling(parent);
		Node* grand_parent = parent->parent;
		uncle->color = Node::Color::black;
		parent->color = Node::Color::black;
		if (grand_parent != root)
		grand_parent->color = Node::Color::red;
		/*SwitchColor(uncle);
		SwitchColor(parent);
		SwitchColor(grand_parent);*/
	}

	/*Switches the color of the node.*/
	void SwitchColor(Node* node)
	{
		if (node->color == Node::Color::black && node != root)
		{
			node->color = Node::Color::red;
		}
		else
		{
			node->color = Node::Color::black;
		}
	}

	Node* InOrderSuccessor(Node* node)
	{
		if (node->right != nullptr)
		{
			node = node->right;
		}
		else
		{
			return node;
		}
		while (node->left != nullptr)
		{
			node = node->left;
		}
		return node;
	}

	/*Returns the node with the smallest key.*/
	Node* Minimum()
	{
		Node* node = root;
		while (node->left != nullptr)
		{
			node = node->left;
		}
		return node;
	}

	/*Returns the node with the largest key.*/
	Node* Maximum()
	{
		Node* node = root;
		while (node->right != nullptr)
		{
			node = node->right;
		}
		return node;
	}

	void InOrderTraversalPrint(std::ostream& stream, Node* node)
	{
		if (node == nullptr)
		{
			return;
		}
		InOrderTraversalPrint(stream, node->left);
		stream << "(" << node->key << ", " << node->content << ") ";
		InOrderTraversalPrint(stream, node->right);
	}

	void GetInOrderArray(std::tuple<T, U> arr[], Node* node, int& place)
	{
		if (node == nullptr)
		{
			return;
		}
		GetInOrderArray(arr, node->left, place);
		arr[place] = std::tuple<T, U>(node->content, node->key);
		place++;
		GetInOrderArray(arr, node->right, place);
	}

	void GetInOrderPointerArray(Node** arr, Node* node, int& place)
	{
		if (node == nullptr)
		{
			return;
		}
		GetInOrderPointerArray(arr, node->left, place);
		arr[place] = node;
		place++;
		GetInOrderPointerArray(arr, node->right, place);
	}

	int GetHeight(Node* node)
	{
		if (node == nullptr)
		{
			return -1;
		}
		int lhs = GetHeight(node->left);
		int rhs = GetHeight(node->right);
		return std::max(lhs, rhs) + 1;
	}

public:

	~RedBlackTree()
	{
		Clear();
	}

	static class RBIterator : public std::iterator<
		std::random_access_iterator_tag,
		T,
		std::ptrdiff_t,
		Node*,
		T&>
	{
		int index;
		RedBlackTree* tree;
	public:
		typedef RBIterator iterator;
		RBIterator(Node* other, RedBlackTree* _tree, int _index = -1)
		{
			tree = _tree;
			if (_tree->sorted_pointers == nullptr)
			{
				_tree->sorted_pointers = new Node*[_tree->GetSize()];
				int place = 0;
				_tree->GetInOrderPointerArray(_tree->sorted_pointers
					, _tree->root, place);
			}
			if (_index == -1)
			{
				for (int i = 0;i < _tree->GetSize();i++)
				{
					if (_tree->sorted_pointers[i] == other)
					{
						index = i;
					}
				}
			}
			else
			{
				index = _index;
			}
		}
		~RBIterator() = default;

		iterator& operator++()
		{
			if (index == tree->GetSize())
			{
				throw std::out_of_range("The iterator can't point further than End()!");
			}
			else
			{
				index++;
			}
			return *this;
		}

		iterator operator++(ptrdiff_t num)
		{
			if (index == tree->GetSize())
			{
				throw std::out_of_range("The iterator can't point further than End()!");
			}
			else
			{
				index++;
			}
			return iterator(tree->sorted_pointers[index - 1], tree, index - 1);
		}

		iterator& operator--()
		{
			if (index == 0)
			{
				throw std::out_of_range("The iterator can't point before Begin()!")
			}
			else
			{
				index--;
			}
			return *this;
		}

		iterator operator--(ptrdiff_t num)
		{
			if (index == 0)
			{
				throw std::out_of_range("The iterator can't point before Begin()!")
			}
			else
			{
				index--;
			}
			return iterator(&tree->sorted_pointers[index + 1], tree, index + 1);
		}

		iterator& operator+=(ptrdiff_t num)
		{
			if (index + num > tree->GetSize())
			{
				throw std::out_of_range("The iterator can't point further than End()!");
			}
			else
			{
				index += num;
			}
			return *this;
		}

		iterator& operator-=(ptrdiff_t num)
		{
			if (index - num < 0)
			{
				throw std::out_of_range("The iterator can't point before Begin()!");
			}
			else
			{
				index -= num;
			}
			return *this;
		}

		iterator& operator+(ptrdiff_t num)
		{
			if (index + num > tree->GetSize())
			{
				throw std::out_of_range("The iterator can't point further than End()!");
			}
			return iterator(tree->sorted_pointers[index + num], tree, index + num);
		}

		iterator& operator-(ptrdiff_t num)
		{
			if (index - num < 0)
			{
				throw std::out_of_range("The iterator can't point before Begin()!");
			}
			return iterator(tree->sorted_pointers[index - num], tree, index - num);
		}

		T& operator*()
		{
			if (index == tree->GetSize())
			{
				throw std::out_of_range("Cannot dereference End()!");
			}
			return (tree->sorted_pointers[index])->content;
		}

		bool operator==(const iterator& other)
		{
			if ((other.index == other.tree->GetSize())
				^ (index == tree->GetSize()))
			{
				return false;
			}
			else if (index == tree->GetSize() && other.tree->GetSize() == other.index)
			{
				return true;
			}
			return *this == other;
		}

		bool operator!=(const iterator& other)
		{
			return !(*this == other);
		}

		bool operator<(const iterator& rhs)
		{
			if (rhs.index == rhs.tree->GetSize()
				^ index == tree->GetSize())
			{
				if (index == tree->GetSize())
				{
					return false;
				}
				return true;
			}
			return tree->sorted_pointers[index]->key
				< rhs.tree->sorted_pointers[rhs.index]->key;
		}

		bool operator>(const iterator& rhs)
		{
			if (rhs.index == rhs.tree->GetSize()
				^ index == tree->GetSize())
			{
				if (index == tree->GetSize())
				{
					return true;
				}
				return false;
			}
			return tree->sorted_pointers[index]->key
		> rhs.tree->sorted_pointers[index]->key;
		}

		bool operator<=(const iterator& rhs)
		{
			if (rhs.index == rhs.tree->GetSize()
				^ index == tree->GetSize())
			{
				if (index == tree->GetSize())
				{
					return false;
				}
				return true;
			}
			if (rhs.index == rhs.tree->GetSize()
				&& index == tree->GetSize())
			{
				return true;
			}
			return tree->sorted_pointers[index]->key
				<= rhs.tree->sorted_pointers[rhs.index]->key;
		}

		bool operator>=(const iterator& rhs)
		{
			if (rhs.index == rhs.tree->GetSize()
				^ index == tree->GetSize())
			{
				if (index == tree->GetSize())
				{
					return true;
				}
				return false;
			}
			if (rhs.index == rhs.tree->GetSize()
				&& index == tree->GetSize())
			{
				return true;
			}
			return tree->sorted_pointers[index]->key
				>= rhs.tree->sorted_pointers[rhs.index]->key;
		}
	};

	typedef RBIterator iterator;

	size_t GetSize()
	{
		return size;
	}

	/*Returns the smallest key in the container.*/
	U MinimumKey()
	{
		if (GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Minimum()->key;
	}

	/*Returns the largest key in the container.*/
	U MaximumKey()
	{
		if (GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Maximum()->key;
	}

	/*Returns the object with smallest key in the container.*/
	T MinimumVal()
	{
		if (GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Minimum()->content;
	}

	/*Returns the object with largest key in the container.*/
	T MaximumVal()
	{
		if (GetSize() == 0)
		{
			throw std::range_error("This container has no contents.");
		}
		return Maximum()->content;
	}

	/*Inserts the element into the tree.Doesn't do anything if the key
	 is already present in the tree.*/
	void Insert(T element, U _key)
	{
		if (sorted_pointers != nullptr)
		{
			delete sorted_pointers;
			sorted_pointers = nullptr;
		}
		/*Create and place the node into the tree.*/
		Node* node = new Node();
		node->content = element;
		node->color = Node::Color::red;
		node->key = _key;
		int old_size = GetSize();
		if (GetSize() == 0)
		{
			node->color = Node::Color::black;
		}
		size++;
		InitiallyPlace(node);
		/*Restructure the node to make sure that
		there's no red node that parents another red
		node.*/
		if (node != root && old_size != GetSize())
		{
			RestructureAfterInsert(node);
		}
	}

	/*Removes the param key and its associated object from
	the tree.Returns false if the key wasn't present in the tree.*/
	bool Remove(const U& key)
	{
		Node* node = FindNode(key);
		if (node == nullptr)
		{
			return false;
		}
		else if (node == root && GetSize() == 1)
		{
			Clear();
		}
		else
		{
			if (sorted_pointers != nullptr)
			{
				delete sorted_pointers;
				sorted_pointers = nullptr;
			}
			RemoveGroup r = InitiallyDelete(node);
			if(GetSize() > 1)
			RestructureAfterDeletion(r.node, r.sibling,r.parent, r.color);
		}
		return true;
	}

	/*Gets the height of the tree.
	Returns -1 if there is no root.*/
	int GetHeight()
	{
		return GetHeight(root);
	}

	/*Clears the tree of all its nodes.*/
	void Clear()
	{
		if (root != nullptr)
		{
			delete root;
			root = nullptr;
			size = 0;
		}
		if (sorted_pointers != nullptr)
		{
			delete sorted_pointers;
			sorted_pointers = nullptr;
		}
	}

	RBIterator Begin()
	{
		return iterator(nullptr, this, 0);
	}

	const RBIterator CBegin()
	{
		return const Begin();
	}

	RBIterator End()
	{
		return iterator(nullptr, this, GetSize());
	}

	const RBIterator CEnd()
	{
		return const End();
	}

	/*Returns the element that has the specified
	key.*/
	iterator Find(const U& _key)
	{
		Node* node = FindNode(_key);
		if (node == nullptr)
		{
			return End();
		}
		else
		{
			return iterator(node, this);
		}
	}

	bool ContainsVal(T& val)
	{
		for (iterator i = CBegin();i != CEnd();i++)
		{
			if (*iter == val)
			{
				return true;
			}
		}
		return false;
	}

	/*Returns whether a key is present in the tree.*/
	bool ContainsKey(const U& key)
	{
		return Find(key) != End();
	}


	friend std::ostream& operator<<(std::ostream& stream, RedBlackTree& tree)
	{
		tree.InOrderTraversalPrint(stream, tree.root);
		return stream;
	}

	/*Gets an array of tuples sorted by the keys (O(N)).
	NOTE: the array is allocated on heap,so call delete[]
	when you're done using it.*/
	std::tuple<T, U>* ToSortedArray()
	{
		std::tuple<T, U>* arr = new std::tuple<T, U>[GetSize()];
		int place = 0;
		GetInOrderArray(arr, root, place);
		return arr;
	}

};


