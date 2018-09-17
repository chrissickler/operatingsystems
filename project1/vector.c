#include <stdlib.h>
#include "vector.h"

struct vectorStr {
    int length;
    int size;
    void** arr;
    void (*del)(void *);
}

Vector* VectorInit(int length) {
    Vector* vec = malloc(sizeof(Vector));
    vec->length = 0;
    vec->size = length;
    vec->arr = malloc(sizeof(void*) * length);    // Allocate array space
    for(int i = 0; i < length; i++)
    {
        vec->array[i] = 0x0;    // Null init
    }
    vec->del = free;
    return vec;
}

Vector* VectorNewReg(int length, void (*func)(void *)) {
    Vector* vec = VectorInit(length);
    VectorRegDel(vec,func);
    return vec;
}

void VectorRegDel(Vector *vec, void (*func)(void *)) {
    vec->del = func;
}

void VectorDeleteFull(Vector* vec) {
    int length = vec->length;
    for(int i = 0; i < length; i++) {
        if (vec->arr[i] != 0) {
            vec->del(vec->arr[i]);
        }
        VectorDeleteLite(vec);
    }
}

void VectorDeleteLite(Vector* vec) {
    free(vec->arr);
    free(vec);
}

void VectorDelete(Vector* vec) {
    VectorDeleteLite(vec);
}

void VectorRealloc(Vector* this, in newSize) {
    if(size <= this->length)
        return;
    void** oldArr = this->array;
    int oldSize = this->size;
    int newSize = size;

    void** newArr = malloc(sizeof(void*) * newSize);
    int i = 0;
    for(; i < oldSize; i++)
    {
        newArr[i] = oldArr[i];
    }
    for(; i < newSize; i++)
    {
        newArr[i] = 0x0;
    }

    this->arr = newArr;
    this->size = newSize;
    free(oldArr);
}

void VectorPush(Vector *vec, void* ptr) {
    if (vec->length == vec->size) {
        VectorRealloc(vec,vec->size*2);
    }
    vec->arr[vec->length] = ptr;
    vec->length++;
}

void* VectorPop(Vector* vec) {
    if(vec->length > 0) {
        vec->length -= 1;
        return vec->arr[vec->length];
    }
    return 0x0;
}

void* VectorGet(Vector* this, int index) {
    if(index < this->length)
        return this->arr[index];
    return 0x0;
}

int VectorFind(Vector* this, void * thing) {
    int length = this->length;
    void **arr = this->arr;
    for( int i = 0; i < length; i++ ) {
        if(thing == arr[i]) {
            return i;
        }
    }
    return -1;
}

void VectorRemoveAt(Vector* this, int index ) {
    int length = this->length;
    if( index < 0 || length <= index || length < 1 )
        return;

    void **arr = this->arr;
    for( int i = index; i < length-1; i++ ) {
        arr[i] = arr[i+1];
    }
    arr[length-1] = NULL;
}

void VectorRemove(Vector* this, void * thing) {
    int loc = Vector_find(this, thing);
    Vector_removeAt(this, loc);
}

const void * const * VectorToArray(Vector* this) {
    return this->arr;
}

void** VectorToArrayCpy(Vector* this) {
    int length = this->length;
    void** output = malloc(sizeof(void*) * length);
    void** array = this->arr;
    for(int i = 0; i < length; i++)
    {
        output[i] = array[i];
    }
    return output;
}

int VectorLength(Vector* this) {
    return this->length;
}

int VectorSize(Vector* this) {
    return this->size;
}
