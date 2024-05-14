#include<iostream>
#include<stdio.h>

using namespace std;

template <class T>
struct Node
{
	T data ;
	struct Node *next;
	
};

/*
-------------------------------------------------------------------------
  -> Singly linear Linked list
-------------------------------------------------------------------------

*/

template <class T>
class SinglyLL
{
	public :
	struct Node<T> *Head ;
	int count ;
	
	SinglyLL();
	
	void Display();
	int iCount();
	
	void InsertFirst(T);
	void InsertLast(T);
	void InsertAtPos(T,int);
	
	void DeleteFirst();
	void DeleteLast();
	void DeleteAtPos(int);
	
	
};

template <class T>
SinglyLL<T> :: SinglyLL()
{
	Head = NULL;
	count = 0;
}

template <class T>
void SinglyLL<T> :: InsertFirst(T no)
{
	Node <T> *temp = new Node<T>;
	
	temp->data = no;
	temp->next = NULL;
	
	if(Head == NULL)
	{
		Head = temp;
	}else
	{
		temp->next = Head;
		Head = temp;
	}
	count++;
}

template <class T>
void SinglyLL<T> :: Display()
{
	Node <T> *temp = Head;
	
	while(temp != NULL)
	{
		cout<<"|"<<temp->data<<"|->";
		temp = temp->next;
	}
}

template <class T>
void SinglyLL <T> :: InsertLast(T no)
{
	Node <T> *temp = new Node<T>;
	
	temp->data = no;
	temp->next = NULL;
	
	if(Head == NULL)
	{
		Head = temp;
	}else
	{
		Node <T> * temp1 = Head;

		while(temp1->next  != NULL)
		{
			temp1 = temp1->next;
		}
		
		temp1->next = temp;
	}
	count++;
}

template <class T>
void SinglyLL <T>:: InsertAtPos(T data,int pos )
{
	int cnt = 0;
	if(pos == 1)
	{
		InsertFirst(data);
	}else if(pos == (count + 1))
	{
		InsertLast(data);
	}else{
		Node <T> * temp = Head;
		for(int i = 1 ; i < (pos - 1) ; i++)
		{
			temp = temp->next;
		}
		
		Node <T> * newn = new Node<T>;
		
		newn->data = data;
		newn->next = NULL;
		
		//store the node at appropriate position
		newn->next = temp->next;
		temp->next = newn;	
		
		count++;
	}
	
}


template <class T>
void SinglyLL<T> :: DeleteFirst()
{
	if(Head == NULL)
	{
		return;
	}else if(Head->next == NULL)
	{
		delete Head;
		Head = NULL;
	}else{
		Node <T> *temp = Head;
		
		Head = Head->next;
		
		delete temp;
		
	}
	count--;
}

template <class T>
void SinglyLL <T>::DeleteLast()
{
	if(Head == NULL)
	{
		return;
	}else if(Head->next == NULL)
	{
		delete Head;
		Head = NULL;
	}else
	{
		Node <T> *temp = Head;
		while(temp->next->next != NULL)
		{
			temp = temp->next;
		}
		
		delete temp->next;
		temp->next = NULL;
	}
	count--;
}

template <class T>
void SinglyLL<T> :: DeleteAtPos(int pos)
{
	if(pos == 1)
	{
		DeleteFirst();
	}else if(pos == count)
	{
		DeleteLast();
	}else{
		Node <T> * temp = Head;
		for(int i = 1 ; i < (pos - 1) ; i++ )
		{
			temp = temp->next;
		}
		
		Node <T> * temp1 = temp->next;
		
		temp->next = temp->next->next;
		
		delete temp1;
	}	
}



/*
-------------------------------------------------------------------------
   ->double linear linked list

template <class T>
struct NodeD
{
	T data ;
	struct NodeD *next;
	struct NodeD *prev;
	
};
-------------------------------------------------------------------------

*/

template <class T>
struct NodeD
{
	T data ;
	struct NodeD *next;
	struct NodeD *prev;
	
};


template <class T>
class DoublyLL
{
	public :
	struct NodeD<T> *Head ;
	struct NodeD<T> *Tail;
	int count ;
	
	DoublyLL();
	
	void Display();
	void DisplayR();
	int iCount();
	
	void InsertFirst(T);
	void InsertLast(T);
	void InsertAtPos(T,int);
	
	void DeleteFirst();
	void DeleteLast();
	void DeleteAtPos(int);
	
	
};

template <class T>
DoublyLL <T> :: DoublyLL()
{
	Head = NULL;
	Tail = NULL;
	count = 0;
}

template <class T>
void DoublyLL <T> :: InsertFirst(T data)
{
	struct NodeD <T> *newn = new NodeD<T>;
	newn->data = data;
	newn->next = NULL;
	newn->prev = NULL;
	
	if((Head == NULL) && (Tail == NULL))
	{
		Head = newn;
		Tail = newn;
	}else {
		newn->next = Head;
		Head->prev = newn;
		
		Head = newn;
	}	
	count++;
}

template <class T>
void DoublyLL <T> :: Display( )
{
	NodeD <T> *temp = Head;
	while(temp != NULL)
	{
		cout<<"|"<<temp->data<<"|->";
		temp = temp->next;
	}
}

template <class T>
void DoublyLL <T> :: DisplayR()
{
	NodeD <T> *temp = Tail;
	while(temp != NULL)
	{
		cout<<"|"<<temp->data<<"|->";
		temp = temp->prev;
	}
}
template <class T>
void DoublyLL <T> :: InsertLast(T data )
{
	NodeD <T> *newn = new NodeD<T>;
	newn->data = data;
	newn->prev = NULL;
	newn->next = NULL;
	
	if((Head == NULL) && (Tail == NULL))
	{
		Head = newn;
		Tail = newn;
	}else{
		Tail->next = newn;
		newn->prev = Tail;
		
		Tail = Tail->next;
	}
	count++;
}


template <class T>
void DoublyLL <T> :: InsertAtPos(T data , int pos)
{
	if(pos == 1)
	{
		InsertFirst(data);
	}else if(pos == (count + 1))
	{
		InsertLast(data);
	}else {
		NodeD <T> * temp = Head;
		for(int i = 1 ; i < (pos -1 ) ;i++)
		{
			temp = temp->next;
		}
		
		//allocate the reasource
		NodeD <T> *newn = new NodeD<T>;
		newn->data = data;
		newn->next = NULL;
		newn->prev = NULL;
		//right
		newn->next = temp->next;
		temp->next->prev = newn;
		
		//left
		temp->next = newn;
		newn->prev = temp;
		
		count++;
	}
}


template <class T>
void DoublyLL <T> :: DeleteFirst()
{
	if((Head == NULL) && (Tail == NULL))
	{
		return ;
	}else if((Head->next == NULL) && (Tail->next == NULL))
	{
		delete Head;
		Head = NULL;
		Tail = NULL;
	}else{
		Head = Head->next;
		
		delete Head->prev;
		
		Head->prev = NULL;
	}
	count--;

}


template <class T>
void DoublyLL <T> :: DeleteLast()
{
	if((Head == NULL)&&(Tail == NULL))
	{
		return;
	}else if((Head->next == NULL)&&(Tail->next == NULL))
	{
		delete Head;
		Head = NULL;
		Tail = NULL;
	}
	else {
	//moving the tail one step back
		Tail = Tail->prev;
	//deleting the last node
		delete Tail->next;
	//make the tail->next = NULL;
		Tail->next = NULL;
	}
	count--;
}



template <class T>
void DoublyLL <T> :: DeleteAtPos(int pos)
{
	if(pos == 1)
	{
		DeleteFirst();
	}else if(pos == count)
	{
		DeleteLast();
	}else{
		NodeD <T> *temp = Head;
		for(int i = 1 ; i < (pos - 1) ; i++)
		{
			temp = temp->next;
		}
		
		temp->next = temp->next->next;
		
		delete temp->next->prev;
		temp->next->prev = temp;
	}
}

/*
-------------------------------------------------------------------------
   ->double Circular linked list

template <class T>
struct NodeD
{
	T data ;
	struct NodeD *next;
	struct NodeD *prev;
	
};
-------------------------------------------------------------------------

*/

template <class T>
class DoublyCL
{
	public :
	struct NodeD<T> *Head ;
	struct NodeD<T> *Tail;
	int count ;
	
	DoublyCL();
	
	void Display();
	void DisplayR();
	int iCount();
	
	void InsertFirst(T);
	void InsertLast(T);
	void InsertAtPos(T,int);
	
	void DeleteFirst();
	void DeleteLast();
	void DeleteAtPos(int);
	
	
};

template <class T>
DoublyCL <T> :: DoublyCL()
{
	Head = NULL;
	Tail = NULL;
	count = 0;
}

template <class T>
void DoublyCL <T> ::InsertFirst(T data)
{
	NodeD <T> * newn = new NodeD<T>;
	newn->data = data;
	newn->next = NULL;
	newn->prev = NULL;
	
	if((Head == NULL)&& (Tail == NULL))
	{
		Head = newn;
		Tail = newn;
		
		Tail->next = Head;
		Head->prev = Tail;
	}else{
		newn->next = Head;
		Head->prev = newn;
		
		Head = newn;
		
		Tail->next = Head;
		Head->prev = Tail;
	}
	count++;
}

template <class T>

void DoublyCL <T>:: Display()
{
	NodeD <T> *temp = Head;
	
	do
	{
		cout<<"|"<<temp->data<<"|->";
		temp = temp->next;
		
	}while(temp != Head);
}


template <class T>

void DoublyCL <T>:: DisplayR()
{
	NodeD <T> *temp = Tail;
	
	do
	{
		cout<<"|"<<temp->data<<"|->";
		temp = temp->prev;
		
	}while(temp != Tail);
}

template <class T>

void DoublyCL <T>:: InsertLast( T no )
{
	NodeD <T> * newn = new NodeD<T> ;
	newn->data = no;
	newn->next = NULL;
	newn->prev = NULL;
	
	
	if((Head == NULL)&&(Tail == NULL))
	{
		Head = newn;
		Tail = newn;
		
		Tail->next = Head;
		Head->prev = Tail;
	}else
	{	
		
		Tail->next = newn;
		newn->prev = Tail;
		
		Tail = Tail->next;
		
		Tail->next = Head;
		Head->prev = Tail;		
	}
	count++;
}

template <class T>

void DoublyCL <T>:: InsertAtPos( T no , int pos)
{
	
	
	
	if(pos == 1)
	{
		InsertFirst(no);
	}else if(pos == (count + 1))
	{
		InsertLast(no);
	}else
	{
		NodeD <T> * temp = Head;
		for(int i = 1 ;i < (pos - 1) ; i++)
		{
			temp = temp->next;
		}
		
		//allocate the memory
		NodeD <T> * newn = new NodeD<T> ;
		
		//inilize the members
		newn->data = no;
		newn->next = NULL;
		newn->prev = NULL;
		
		//right linkage
		newn->next = temp->next;
		temp->next->prev = newn;
		
		//left linkage
		temp->next = newn;
		newn->prev = temp;		
		
	}
	count++;
}


template <class T>

void DoublyCL <T>:: DeleteFirst( )
{	
	if((Head == NULL)&&(Tail == NULL))
	{
		return;
	}else if((Head->next == Head)&&(Tail->prev = Tail))
	{
		delete Head;
		Head = NULL;
		Tail = NULL;
	}else{
	
		Head = Head->next;
		delete Tail->next;
		
		Tail->next = Head;
		Head->prev = Tail;
	}
	count--;
}

template <class T>

void DoublyCL <T>:: DeleteLast( )
{	
	if((Head == NULL)&&(Tail == NULL))
	{
		return;
	}else if((Head->next == Head)&&(Tail->prev = Tail))
	{
		delete Head;
		Head = NULL;
		Tail = NULL;
	}else{
	
		Tail = Tail->prev ;
		
		delete Tail->next;
		
		Tail->next = Head;
		Head->prev = Tail;
	}
	count--;
}

template <class T>

void DoublyCL <T>:: DeleteAtPos(int pos )
{	
	if(pos == 1)
	{
		DeleteFirst();
	}else if(pos == count)
	{
		DeleteLast();
	}else {
		NodeD <T> * temp = Head;
		for(int i = 1 ; i < (pos - 1 ); i++)
		{
			temp = temp->next;
		}
		temp->next = temp->next->next;
		
		delete temp->next->prev;
		temp->next->prev = temp;
	}
	count--;
}



/*
-------------------------------------------------------------------------
   ->singly circular Linked list

template <class T>
struct Node
{
	T data ;
	struct NodeD *next;
	
};
-------------------------------------------------------------------------

*/

template <class T>
class DoublyCS
{
	public :
	struct Node<T> *Head ;
	int count ;
	
	DoublyCS();
	
	void Display();
	void DisplayR();
	int iCount();
	
	void InsertFirst(T);
	void InsertLast(T);
	void InsertAtPos(T,int);
	
	void DeleteFirst();
	void DeleteLast();
	void DeleteAtPos(int);
	
	
};

template <class T>
DoublyCS <T> :: DoublyCS()
{
	Head = NULL;
	count = 0;
}

template <class T>
void DoublyCS <T> :: InsertFirst(T data)
{
	//allocate the memory
	Node <T> *newn = new Node<T>;
	newn->next = NULL;
	newn->data = data;
	
	
	if(Head == NULL)
	{
		Head = newn;
	}else{
		newn->next = Head;
		Head = newn;
	}
}

template <class T>
void DoublyCS <T> :: Display()
{
	
}

template <class T>
void DoublyCS <T> :: DisplayR()
{
	
}
template <class T>
void DoublyCS <T> :: InsertFLast()
{
	
}
template <class T>
void DoublyCS <T> :: InsertAtPos()
{
	
}

template <class T>
void DoublyCS <T> :: DeletFirst()
{
	
}
template <class T>
void DoublyCS <T> :: DeleteLast()
{
	
}

template <class T>
void DoublyCS <T> :: DeletAtPos()
{
	
}
int main()
{
	/*SinglyLL <char> sobj;
	
	sobj.InsertFirst('s');
	sobj.InsertFirst('i');
	sobj.InsertFirst('R');
		
	sobj.Display();	
	
	cout<<("\n---------------------new-------------------------\n");
	sobj.InsertLast('h');
	sobj.InsertLast('v');
	
	sobj.Display();	
	cout<<("\n---------------------new-------------------------\n");
	sobj.InsertAtPos('i',6);
	sobj.InsertAtPos('B',3);
	
	
	sobj.Display();		
	
	cout<<("\n---------------------Deletefirst-------------------------\n");
	sobj.DeleteFirst();
	sobj.DeleteFirst();	
	sobj.Display();	
	
	cout<<("\n---------------------Deletelast-------------------------\n");
	sobj.DeleteLast();
	sobj.DeleteLast();	
	sobj.Display();	
	
	cout<<("\n---------------------DeleteAtPos-------------------------\n");
	sobj.DeleteAtPos(2);	
	sobj.Display();		*/
	
	/*
	DoublyLL <char> dobj ;
	
	cout<<("\n---------------------Insertfirst-------------------------\n");
	dobj.InsertFirst('s');
	dobj.InsertFirst('i');	
	dobj.InsertFirst('R');
	dobj.Display();	
	cout<<("\n---------------------reverse-------------------------\n");
	dobj.DisplayR();
	cout<<("\n---------------------InsertLast-------------------------\n");
	dobj.InsertLast('h');
	dobj.InsertLast('v');
	dobj.Display();	
	cout<<("\n---------------------reverse-------------------------\n");
	dobj.DisplayR();

	cout<<("\n---------------------InsertAtpos-------------------------\n");
	dobj.InsertAtPos('B',6);
	dobj.Display();
	cout<<"\n";
	dobj.InsertAtPos('I',3);
	dobj.Display();
	
	
	cout<<"\n--------------------------DeleteFirst-------------------------------------";
	dobj.DeleteFirst();
	dobj.Display();
	dobj.DisplayR();
	cout<<"\n";
	dobj.DeleteLast();
	dobj.Display();
	dobj.DisplayR();
	
	cout<<"\n--------------------------DeleteAtpos-------------------------------------";
	dobj.DeleteAtPos(3);
	dobj.DeleteAtPos(1);
	dobj.Display();
	dobj.DisplayR();*/
	
	/*DoublyCL <char> dobj ;
	
	cout<<"insert First \n";
	dobj.InsertFirst('s');
	dobj.InsertFirst('I');
	dobj.InsertFirst('R');
	dobj.Display();
	cout<<"\n";
	dobj.DisplayR();

	dobj.InsertLast('H');
	dobj.InsertLast('V');
	dobj.Display();
	cout<<"\n";
	dobj.DisplayR();
	dobj.Display();
	cout<<"\n";
	dobj.DisplayR();
	dobj.InsertAtPos('i' , 3);
	
	
	
	dobj.Display();	
	cout<<"Delete first";
	dobj.DeleteFirst();
	dobj.Display();
	cout<<"\n";
	dobj.DisplayR();
	
	cout<<"Delete Last\n";
	dobj.DeleteLast();
	dobj.Display();
	cout<<"\n";
	dobj.DisplayR();
	
	
	cout<<"\n Delete At pos";
	dobj.DeleteAtPos(3);
	dobj.Display();
	cout<<"\n";
	dobj.DisplayR();*/
	
	return 0;
}






