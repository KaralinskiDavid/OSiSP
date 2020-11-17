#include "pch.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "PhoneBookLine.h"

using namespace std;

template<typename K, typename D>
class Node {
public:
	K key;
	D* data;
	vector<D*> samekeydata;

	Node* left;
	Node* right;
	Node* parent;
	Node* newNode(K key, D* data, Node* parent);
	void insert(Node** head, K key, D* data);
	vector<Node*> getNodesByKey(Node* root, K key);
};

template<typename K, typename D>
Node<K,D>* Node<K, D>::newNode(K key, D* data, Node* parent)
{
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp->left = tmp->right = NULL;
	tmp->parent = parent;
	tmp->key = key;
	tmp->data = data;
	return tmp;
}

template<typename K, typename D>
void Node<K, D>::insert(Node** head, K key, D* data)
{
	Node* tmp = NULL;
	Node* ins = NULL;
	if (*head == NULL)
	{
		*head = Node::newNode(key, data, NULL);
		return;
	}

	tmp = *head;
	while (tmp)
	{
		if (key > tmp->key)
		{
			if (tmp->right)
			{
				tmp = tmp->right;
				continue;
			}
			else
			{
				tmp->right = Node::newNode(key, data, tmp);
				return;
			}
		}
		else if (key < tmp->key)
		{
			if (tmp->left)
			{
				tmp = tmp->left;
				continue;
			}
			else
			{
				tmp->left = Node::newNode(key, data, tmp);
				return;
			}
		}
		else if (key == tmp->key)
		{
			tmp->samekeydata.push_back(data);
			return;
		}
	}

}

template<typename K, typename D>
vector<Node<K, D>*> Node<K,D>::getNodesByKey(Node* root, K key)
{
	while (root)
	{
		if (root->key > key)
		{
			root = root->left;
			continue;
		}
		else if (root->key < key)
		{
			root = root->right;
			continue;
		}
		else
		{
			vector<Node*> lines = root->samekeydata;
			lines.push_back(root);
			return lines;
		}
	}
	return NULL;
}


vector<wstring> splitline(wstring line, wstring delimiter)
{
	vector<wstring> substrings;
	size_t prev = 0;
	size_t next;
	size_t delta = delimiter.length();

	while ((next = line.find(delimiter, prev)) != string::npos)
	{
		substrings.push_back(line.substr(prev, next - prev));
		prev = next + delta;
	}
	substrings.push_back(line.substr(prev));

	return substrings;
}

Node<wstring, PhoneBookLine>* StreetIndex = NULL;
Node<wstring, PhoneBookLine>* LastnameIndex = NULL;
Node<wstring, PhoneBookLine>* PhonenumberIndex = NULL;


extern __declspec(dllexport) vector<PhoneBookLine*> loadPhonebook(wstring path)
{
	wifstream in(path);
	vector<wstring> substrings;
	wstring line;
	wstring delimiter = wstring(L" ");
	PhoneBookLine* phonebookElement;
	vector<PhoneBookLine*> phonebook;

	if (in.is_open())
	{
		while (getline(in, line))
		{
			try 
			{
				substrings = splitline(line, delimiter);
				phonebookElement = new PhoneBookLine(
					substrings[0],
					substrings[1],
					substrings[2],
					substrings[3],
					substrings[4],
					_wtoi(substrings[5].c_str()),
					_wtoi(substrings[6].c_str()),
					_wtoi(substrings[7].c_str())
				);
				phonebook.push_back(phonebookElement);
				StreetIndex->insert(&StreetIndex, phonebookElement->street, phonebookElement);
				LastnameIndex->insert(&LastnameIndex, phonebookElement->lastname, phonebookElement);
				PhonenumberIndex->insert(&PhonenumberIndex, phonebookElement->phonenumber, phonebookElement);
			}
			catch(...)
			{

			}
		}

		in.close();

		return phonebook;
	}

}

enum IndexTypes
{
	STREET_INDEX,
	LASTNAME_INDEX,
	PHONENUMBER_INDEX
};

template<typename K>
extern __declspec(dllexport) vector<PhoneBookLine*> searchByIndex(K index, int indextype)
{
	switch (indextype)
	{
	case IndexTypes.STREET_INDEX:
		return StreetIndex->getNodesByKey(StreetIndex, index);
	case IndexTypes.LASTNAME_INDEX:
		return LastnameIndex->getNodesByKey(LastnameIndex, index);
	case IndexTypes.PHONENUMBER_INDEX:
		return PhonenumberIndex->getNodesByKey(PhonenumberIndex, index);
	}
}

