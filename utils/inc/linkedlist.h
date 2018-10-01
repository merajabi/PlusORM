#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
// C++ program to implement Custom Linked List and
// iterator pattern.

#ifndef nullptr
   #define nullptr 0
#endif

// Custom class to handle Linked List operations
// Operations like PushBack, push_front, pop_back,
// pop_front, erase, size can be added here
template <typename T>
class LinkedList {
    // Forward declaration
    class Node;
    Node* rootNode;
 	long size;
public:
    // Forward declaration must be done
    // in the same access scope
    class Iterator;

    LinkedList<T>()  {
        // caution: static members can't be
        // initialized by initializer list
        rootNode = nullptr;
        //rootNode->data = data;
        //rootNode->pNext = nullptr;
		size=0;
    }

    // Iterator class can be used to
    // sequentially access nodes of linked list
    class Iterator {
		friend class LinkedList;
    private:
        Node* currentNode;
        Node* GetNode(){return currentNode;}
    public:
		Iterator()  : currentNode (nullptr) { }

		Iterator(Node* pNode)  : currentNode (pNode) { }

        Iterator& operator=(Node* pNode) {
            this->currentNode = pNode;
            return *this;
        }

        // Prefix ++ overload
        Iterator& operator++() {
            if (currentNode)
                currentNode = currentNode->pNext;
            return *this;
        }

        // Postfix ++ overload
        Iterator operator++(int) {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }

        bool operator!=(const Iterator& iterator) {
            return currentNode != iterator.currentNode;
        }

        T operator*() {
            return currentNode->data;
        }

    };

    // rootNode of LinkedList wrapped in Iterator type
    Iterator begin() {
        return Iterator(rootNode);
    }

    // End of LInkedList wrapped in Iterator type
    Iterator end() {
        return Iterator(nullptr);
    }

    // Adds data to the end of list
	long Size(){return size;}
    void PushBack(T data);
	void PushFront(T data);
	T PopFirst();
	T PopLast();
	void Insert(T data, long n);
	T Replace(T data, long n);
	T Remove(long n);
	T At(long n);
	T First(){return At(0);};
	T Last(){return At(Size());};

//    void Traverse();

private:

    class Node {
		// LinkedList class methods need
		// to access Node information
		friend class LinkedList;

		T data;
		Node* pNext;
    };

    // Create a new Node
    Node* GetNode(T data) {
        Node* pNewNode = new Node;
        pNewNode->data = data;
        pNewNode->pNext = nullptr;
        return pNewNode;
    }

    // Create a new Node
    T DeleteNode(Node* pNode) {
		T data=pNode->data;
        pNode->pNext = nullptr;
		delete pNode;
        return data;
    }

    // Return by reference so that it can be used in
    // left hand side of the assignment expression
    Node*& GetrootNodeNode() {
        return rootNode;
    }

};

//template <typename T>
// /*static*/ typename LinkedList<T>::Node* LinkedList<T>::rootNode = nullptr;

template <typename T>
void LinkedList<T>::PushBack(T data) {
    Node* pTemp = GetNode(data);
    if (!GetrootNodeNode()) {
        GetrootNodeNode() = pTemp;
    } else {
        Node* pCrawler = GetrootNodeNode();
        while (pCrawler->pNext) {
            pCrawler = pCrawler->pNext;
        }
         pCrawler->pNext = pTemp;
    }
	size++;
}

template <typename T>
void LinkedList<T>::PushFront(T data) {
    Node* pTemp = GetNode(data);
    if (!GetrootNodeNode()) {
        GetrootNodeNode() = pTemp;
    } else {
		pTemp->pNext=GetrootNodeNode()->pNext;
		GetrootNodeNode() = pTemp;
    }
	size++;
}

template <typename T>
T LinkedList<T>::PopFirst() {
	T temp;
    if (GetrootNodeNode()) {
		Node* pTemp=GetrootNodeNode();
		GetrootNodeNode() = pTemp->pNext;
		temp=DeleteNode(pTemp);
		size--;
	}
	return temp;
}


template <typename T>
T LinkedList<T>::PopLast() {
	T temp;
    if (GetrootNodeNode()) {
	    Node* pParent = GetrootNodeNode();
	    Node* pCrawler = pParent->pNext;
		if(pCrawler){
		    while (pCrawler->pNext) {
				pParent = pCrawler;
		        pCrawler = pCrawler->pNext;
		    }
			pParent->pNext=nullptr;
			temp=DeleteNode(pCrawler);
		}else{
			GetrootNodeNode()=nullptr;
			temp=DeleteNode(pParent);
		}
		size--;
    }
	return temp;
}

template <typename T>
T LinkedList<T>::At(long n)	{
	Iterator it;
	long i;
    for ( i=0,it = begin(); i<n && it != end(); ++it, i++);
	return (*it);
}

template <typename T>
void LinkedList<T>::Insert(T data, long n){
	if(n <= 0){
		PushFront(data);
	} else if ( n >= Size() ) {
		PushBack(data);
	} else {
		Iterator it;
		long i;
		for ( i=0,it = begin(); i<n-1 && it != end(); ++it, i++);
		Node* pTemp = GetNode(data);
		Node* pCrawler = it.GetNode();
		pTemp->pNext=pCrawler->pNext;
		pCrawler->pNext=pTemp;
		size++;
	}
}

template <typename T>
T LinkedList<T>::Replace(T data, long n){
	T temp;
	if( n >= 0 && n < Size() ){ // or n <= Size()-1
		temp=Remove(n);
		Insert(data,n);
/*
		Iterator it;
		long i;
		for (i=0,it = begin(); i<n && it != end(); ++it, i++);
		Node* pCrawler = it.GetNode();
		temp=pCrawler->data;
		pCrawler->data=data;
*/
	}
	return temp;
}

template <typename T>
T LinkedList<T>::Remove(long n){
	T temp;
	if(n <= 0){
		PopFirst();
	} else if ( n >= Size() ) {
		PopLast();
	} else {
		Iterator it;
		long i;
		for ( i=0,it = begin(); i<n-1 && it != end(); ++it, i++);
		Node* pParent = it.GetNode();
	    Node* pCrawler = pParent->pNext;
		pParent->pNext=pCrawler->pNext;
		temp=DeleteNode(pCrawler);
		size--;
	}
	return temp;
}
/*
template <typename T>
void LinkedList<T>::Traverse(long n) {
    Node* pCrawler = GetrootNodeNode();
    long i=0;
    if(pCrawler && n < Size() ){
		while (pCrawler->pNext && i<n) {
			 pCrawler = pCrawler->pNext;
			 i++;
		}
    }
    return;
    // cout << endl;
}
*/
#endif //_LINKEDLIST_H_

