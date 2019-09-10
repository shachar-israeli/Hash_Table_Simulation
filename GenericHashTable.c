
#include "GenericHashTable.h"

//
// Created by shachar Israeli on 02/12/18.
//           203713094
//       EX1 - Hash Table.
//

Table *createTable(int size, int dType, int listLength) {

    if(size <= 0 || listLength < 0 ||(dType != INT_TYPE && dType != STR_TYPE))
    {
        perror("ERR: there is a problem with 1 or more parameters\n");
        return NULL;
    }

    Table *newTable = (Table*)malloc(sizeof(Table));
    if (newTable == NULL)
        return NULL;

    newTable->dType = dType;
    newTable->currentSize = size;
    newTable->OriginSize = size;
    newTable->listLength = listLength;
    newTable->arr = (Object**)malloc(sizeof(Object*)*size);

    if (newTable->arr == NULL)
        return NULL;

    for(int i = 0; i < size; i++)    //ini the table with null objects
        newTable->arr[i] = createObject(NULL);

    return newTable;
}

void printTable(Table *table) { // will print the table

  if (table == NULL){
      perror("ERR: there is a problem with 1 or more parameters\n");
      return;
  }

    if ( table->dType == INT_TYPE)  // for int case
        printIntTable(table);
    else
        printStrTable(table); // for str case

}

int intHashFun(int *key, int origSize) {
    return (*key) % origSize;
}

int strHashFun(char *key, int origSize) {
    int sum = 0; // will sum my asci value of the char*
    char* p = key;
    while(*p != '\0')
    {
        sum+= (int)(*p);
        p++;
    }
    return sum % origSize;
}

int isEqual(int type, void *data1, void *data2) {

    if( (type != INT_TYPE && type != STR_TYPE)) {
        perror("ERR: there is a problem with 1 or more parameters\n");
        return ERROR;
    }

    if ( type == INT_TYPE)  // for int case
    {
        if( *(int*)data1 == *((int*)data2) )    // return 0 if its qual
            return 0 ;
        else
            return 1;
    }
    else{

        if(strcmp((char*)data1,(char*)data2) == 0)//compare both the strings
            return 0 ; // match
        else
            return 1;  //not match
    }
}

void freeObject(Object *obj, int type) {

    if(obj == NULL && (type!=INT_TYPE && type != STR_TYPE))
    {
        perror("ERR: there is a problem with 1 or more parameters\n");
        return;
    }

    if( type == STR_TYPE)  // in case of a string the data should be freed also.
        free(obj->data);
    free(obj);
}

Object *createObject(void *data) {

    Object *newObj = (Object*) malloc(sizeof(Object)); // create the obj and put the data in the right place
    if (newObj == NULL)
        return NULL;

    newObj->data = data;
    newObj->next = NULL;

    return newObj;
}

Object *search(Table *table, void *data) {


    if(table == NULL || data == NULL) {
        perror("ERR: there is a problem with 1 or more parameters\n");
        return NULL;

    }

    int index ;
    if( table->dType == INT_TYPE)
        index = intHashFun((int*)data,table->OriginSize);
    else
        index = strHashFun((char*)data,table->OriginSize);

    int d = table->currentSize/table->OriginSize;

    index = d * index;   // the index should be * d when the table is already expanded

    for (int i = 0; i < d ; i++) {

        Object* p = table->arr[index+i]->next; // the start of the linked list
        while ( p != NULL)
        {
           if(isEqual(table->dType,p->data,data) == 0)
               return p;
           else
               p = p->next;  // next if its not match the data u need
        }
    }
    return NULL;  // if doesnt find anything with this data
}

int removeObj(Table *table, void *data) {

    if(table == NULL || data == NULL) {
        perror("ERR: there is a problem with 1 or more parameters\n");
        return ERROR;
    }

    int index;
    if( table->dType == INT_TYPE)
        index = intHashFun((int*)data,table->OriginSize);   // get the index of the object  that u need to remove
    else
        index = strHashFun((char*)data,table->OriginSize);


    int d = table->currentSize/table->OriginSize;   // how many multi*2 we did to the table

    index = d * index ;   // take the right place of the index

    for (int i = 0; i < d ; i++) {

        Object* p = table->arr[index+i]->next;   // get the start of the table
        Object* prev = table->arr[index+i];   // start of the table
        while ( p != NULL)
        {
            if(isEqual(table->dType,p->data,data) == 0)
            {
                prev->next = p->next;    // take the object out of the list and free it
                freeObject(p,table->dType);
                return (index+i);
            }
            else {
                prev = p;    // just go next.
                p = p->next;

            }
        }

    }
    return ERROR;  // if doesnt find the data place to remove.

}

int add(Table *table, void *data) {

    if(table == NULL || data == NULL) {
        perror("ERR: there is a problem with 1 or more parameters\n");
        return ERROR;
    }

    Object* toAdd = NULL;
    if (table->dType == STR_TYPE) {

        char *dataStr = (char*)malloc(sizeof((data)) + 1);  // if its string we need to give it a space in our heap
        if (dataStr == NULL)
            return ERROR;

        strcpy(dataStr,(char*)data);  // copy the value of the string to the heap
         toAdd = createObject(dataStr);
    }
    else  // int case
         toAdd = createObject(data);

    int index = -1;

    if (table->dType == INT_TYPE)
        index = intHashFun(data,table->OriginSize);
    else
        index = strHashFun(data,table->OriginSize);

    int d = table->currentSize/table->OriginSize;  // d  = how many i expand the table

    index = d * index;  // get the right index

    int addIndex = addWithOutExpend(table,toAdd,index);     // return the index if added. if not will get -1

    if (addIndex!= -1)   // mean that we added without expend
        return addIndex;

    else{
        expandTheTable(table);
        movePointers(table);
        addIndex = addWithOutExpend(table,toAdd,2*index);
        return addIndex;
    }

}


void freeTable(Table *table) {

    if(table == NULL) {
        perror("ERR: there is a problem with 1 or more parameters\n");
        return;
    }

    for(int i = 0; i < table->currentSize; i++)
    {
        freeLinkedList(table->arr[i]->next,table->dType);  // will free the list
        freeObject(table->arr[i],table->dType);   // will free the null in the beginning of every string
    }

    free(table->arr);  // free the arr
    free(table);  // free the table object in the end
}


/**  From here , Private methods :D*/

int addWithOutExpend(Table *table, Object* toAdd, int index) {      // return the index of the place we added

    int d = table->currentSize/table->OriginSize;  // d  = how many i expand the table
    int addIndex = -1;

    for (int i = 0; i < d ; i++) {
        int steps = 0; // how many nodes i got. cant be more then table.len
        Object* p = table->arr[index+i];

        while (p && p->next)            /// get the place we can add.
        {
           p = p->next;
           steps++;
        }
        if (steps < table->listLength)  /// mean that i can add!!
        {
            p->next = toAdd; // added to the end of the list
            addIndex = index+i; // we added it ye. gonna return the index of the new object
            return addIndex;
        }

    }
    return addIndex; // will return -1 if nothing added

}

void expandTheTable (Table *table)
{
    int currentSize = 2 * table->currentSize;  // we double the size we had

    table->currentSize = currentSize;
    table->arr = (Object**)realloc(table->arr, currentSize * sizeof(Object*));

    if (table->arr == NULL)
        return;                   // check what to do with this thing

    for(int i = currentSize / 2; i < currentSize; i++)   // need to add object to the new cell's
        table->arr[i] = createObject(NULL);
}

void movePointers (Table *table){

    int currentSize = table->currentSize;

    for(int i = (currentSize / 2) -1; i > 0; i--)   // move the pointers i=0 doesnt need to move so its ok
    {
         Object* temp = table->arr[2*i] ;
         table->arr[2*i] = table->arr[i];   // swap the null place of 2*i <---> i
         table->arr[i] = temp;
    }
}

void printIntTable(Table *table)
{
    for (int i = 0; i < table->currentSize; i++)    // print in case of an INT Table
    {
        printf("[%d]",i);
        Object *temp = table->arr[i]->next;
        while ( temp != NULL)
        {
            printf("\t%d\t-->",*(int*)(temp->data));
            temp = temp->next;
        }
        printf("\n");
    }
}

void printStrTable(Table *table)
{
    for (int i = 0; i < table->currentSize; i++)  // print in case of an STRING Table
    {
        printf("[%d]",i);
        Object *temp = table->arr[i]->next;
        while ( temp != NULL)
        {
            printf("\t%s\t-->",(char*)(temp->data));
            temp = temp->next;
        }
        printf("\n");

    }
}

void freeLinkedList(Object* headList,int type)        // free all grade list.
{
    if (headList == NULL)
        return;
    freeLinkedList(headList->next,type);         // first go to the last node and then start to free the list
    freeObject(headList,type);
}



