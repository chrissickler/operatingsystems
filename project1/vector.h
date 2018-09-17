#ifndef __VECTOR_H__
#define __VECTOR_H__

struct vectorStr;
typedef struct vectorStr Vector;

Vector* VectorInit(int length);
Vector* VectorNewReg(int length, void (*func)(void *));
int VectorLength(Vector* vec);
void VectorRegDel(Vector *vec, void (*func)(void *));
void VectorDeleteFull(Vector* vec);
void VectorDelete(Vector* vec);
void VectorDeleteLite(Vector* vec);

void VectorPush(Vector *vec, void* ptr);
void* VectorPop(Vector *vec);
void VectorRealloc(Vector* vec, in newSize);

void* VectorGet(Vector* vec, int index);
int VectorFind(Vector* vec, void * thing );
void VectorRemoveAt( Vector* vector, int thing );
void VectorRemove( Vector* vector, void * thing );
const void * const * VectorToArray(Vector* this);
void** VectorToArrayCpy(Vector* this);
int VectorSize(Vector* this);

#endif
