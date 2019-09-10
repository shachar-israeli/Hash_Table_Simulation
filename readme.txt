
** Short Brief**

This program implements a data structure of an hash table, and the handling with collisions is done with linked lists. The hash table support these three operations: 1. Adding an element 2. Removing an element 3. Searching for an element. The hash table can be from the type of integers, or from the type of strings. Each type has little different implementation of the hash function. In this program we limit the size of the linked lists that can be made. In case there is an object that the hash function sends to a full linked list, the way this hash table deal with it is by duplicating the hash table size (i.e, expand the table).
In case of expansion, all the existed objects of the table in entries 0-(n-1) will be mapped to the respective even entries in the new hash table.


EX1 - Hashtable.

Dynamic memory Allocation.
our main goal is to implement an hash table that can be dynamic.
in this EX, we use 2 structs:
#"Object": is the objects in the hash table.
#"Table": the hash table.

** EX1 inculdes **

*GenericHashTable.h
*GenericHashTable.c
*makefile
*readme


** install & load **

in linux terminal, go to the folder that containing the Ex.
type terminal: "make"  and the program will automaticily be compiled.
after it. run the program with " ./tester " in terminal. if you want to see my Allocation and heap use, you can write: /valgrind tester


**Program Functions**

	***Public Functions*****

	
1. Table* createTable(int size, int dType, int listLength);

The function gets the original size and the type of the data in the table elements.
it initializes the Table struct members. 

output: On success, the function returns a pointer to the new created Table, otherwise, it return NULL.

2. void freeTable(Table* table);	

The function release all the allocated members of struct Table.

output: none

3.int add(Table* table, void* data);

The function adds data to the hashtable (as described in the exe definition)
output: On success, the function returns the array index of the added data, otherwise, it return -1.
	
4.int removeObj(Table* table, void* data);

 The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
output:On success, the function returns the array index of the removed data, otherwise, it returns -1.

5.Object* search(Table* table, void* data);

The function search for an object that its data is equal to data. 
output:returns a pointer to that object.If there is no such object or in a case of an error, NULL is returned.

6.void printTable(Table* table);
 The function print the table (the format is in the exe definition)
output: print the table

7.Object* createObject(void* data);
This function creates an object.
output: return the pointer to it or NULL if there is some error.
	
void freeObject(Object* obj, int type);

8.int isEqual(int type, void* data1, void* data2);
check the equality of the data of two objects. The implementation is different depending the type of the data.
output:the function returns 0 if they are equal or some other value if they are not equal.

9.int intHashFun(int* key, int origSize);

the hash value of an integer, which is key mod origSize 
output: int, the index in the original table.


10.int strHashFun(char* key, int origSize);
	
hash value of an string, which is m mod origSize, where m is the sum of the ascii value of all the character in key. 
output: int, the index in the origSize table.

	***Private Functions*****  -- my methods
	
1. addWithOutExpend(Table *table, Object* toAdd, int index)

add the object only if we dont need to expend the table.
output: return the index we added.

2.expandTheTable (Table *table)

expand The Table *2 his size. make sure the object stays in the same place here
output - none. return null if something happned

3.movePointers (Table *table)

move the pointer * 2 the origin place. this is the tricky methods to keep all toghther .
output- none

4.printIntTable(Table *table)
print the int table. thats it
output: print the table

5.printStrTable(Table *table)
print the str table.
output: print the string table. 

6.freeLinkedList(Object* headList,int type)
take the first head of every linked list and free. recursive method
output: just free it 1 by 1 .

