

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>


#ifndef __LC_TYPES__
#define __LC_TYPES__

typedef int LCInteger;
typedef uint8_t LCUInteger8Bit;
typedef struct LCString* LCStringRef;
typedef unsigned char LCByte;
typedef bool LCBool;

typedef LCStringRef LCPathRef;
typedef struct LCStore* LCStoreRef;
typedef struct LCStage* LCStageRef;
typedef struct LCPathData* LCPathDataRef;
typedef struct LCData* LCDataRef;
typedef struct LCKeyValue* LCKeyValueRef;
typedef struct LCArray* LCArrayRef;
typedef struct LCTree* LCTreeRef;
typedef struct LCDataStore* LCDataStoreRef;
typedef struct LCCommit* LCCommitRef;
typedef struct LCMutableArray* LCMutableArrayRef;
typedef struct LCDictionary* LCDictionaryRef;
typedef struct LCStoreInterface* LCStoreInterfaceRef;

typedef enum {
  LCData,
  LCTree,
  LCCommit
} LCDataType;

typedef enum {
  LCEqual,
  LCGreater,
  LCSmaller
} LCCompare;

typedef enum {
  LCSuccessTrue,
  LCSuccessFalse
} LCSuccess;

typedef void*(*LCCreateEachCb)(LCInteger i, void* info, void* each);

typedef void(*LCStoreDataCb)(void* storeObj, LCDataType type, char* sha, unsigned char data[], size_t length);
typedef void(*LCDeleteDataCb)(void* storeObj, LCDataType type, char* sha);
typedef size_t(*LCGetDataLengthCb)(void* storeObj, LCDataType type, char* sha);
typedef void(*LCGetDataCb)(void* storeObj, LCDataType type, char* sha, unsigned char buffer[]);

// internal types
typedef struct LCType LCType;
typedef struct LCType* LCTypeRef;
typedef struct LCObjectInfo LCObjectInfo;
typedef struct LCObjectInfo* LCObjectInfoRef;
typedef struct LCHashableObject LCHashableObject;
typedef struct LCHashableObject* LCHashableObjectRef;

struct LCType {
  void (*dealloc)(void* object);
  LCCompare (*compare)(void* object1, void* object2);
  void* meta;
};

struct LCObjectInfo {
  LCInteger rCount;
  LCTypeRef type;
};

struct LCStoreBackend {
  void* storeObject;
  LCStoreDataCb storeCb;
  LCDeleteDataCb deleteCb;
  LCGetDataCb getCb;
  LCGetDataLengthCb getLengthCb;
};

#define LC_SHA1_Length 20
#define LC_SHA1_HEX_Length 41

#endif

#include "LCString.h"
#include "LCKeyValue.h"

#include "LCStore.h"
#include "LCStage.h"
#include "LCTree.h"
#include "LCData.h"
#include "LCSHA.h"
#include "LCUtils.h"
#include "LCDataStore.h"
#include "LCCommit.h"
#include "LCArray.h"
#include "LCMutableArray.h"
#include "LCDictionary.h"
#include "LCMemoryStoreBackend.h"
#include "LCFileStoreBackend.h"
#include "LCStoreInterface.h"
