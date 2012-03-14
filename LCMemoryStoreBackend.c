
#include "LCMemoryStoreBackend.h"

typedef struct LCMemoryStore* LCMemoryStoreRef;

void memoryStoreStoreData(void* store, LCDataType type, char* sha, unsigned char data[], size_t length);
void memoryStoreDeleteData(void* store, LCDataType type, char* sha);
size_t memoryStoreGetDataLength(void* store, LCDataType type, char* sha);
void memoryStoreGetData(void* store, LCDataType type, char* sha, unsigned char buffer[]);

struct LCMemoryStore {
  LCObjectInfo info;
  LCStringRef location;
  LCDictionaryRef dict;
};

static void LCMemoryStoreDealloc(void* object);

LCType typeMemoryStore = {
  .dealloc = LCMemoryStoreDealloc
};

static LCMemoryStoreRef LCMemoryStoreCreate(LCStringRef location) {
  LCMemoryStoreRef newStore = LCNewObject(&typeMemoryStore, sizeof(struct LCMemoryStore));
  newStore->location = LCRetain(location);
  newStore->dict = LCDictionaryCreate(NULL, 0);
  return newStore;
};

static void LCMemoryStoreDealloc(void* object) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)object;
  LCRelease(store->location);
  LCRelease(store->dict);
}

struct LCRepositoryBackend* createLCMemoryStoreBackend(char* location) {
  struct LCRepositoryBackend* backend = malloc(sizeof(struct LCRepositoryBackend));
  if (backend != NULL) {
    LCStringRef locationObj = LCStringCreate(location);
    backend->storeObject = LCMemoryStoreCreate(locationObj);
    LCRelease(locationObj);
    backend->storeCb = memoryStoreStoreData;
    backend->deleteCb = memoryStoreDeleteData;
    backend->getLengthCb = memoryStoreGetDataLength;
    backend->getCb = memoryStoreGetData;
  }
  return backend;
}

void freeLCMemoryStoreBackend(struct LCRepositoryBackend* backend) {
  LCRelease(backend->storeObject);
  free(backend);
}

void memoryStoreStoreData(void* storeObj, LCDataType type, char* sha, unsigned char data[], size_t length) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCDataRef dataObj = LCDataCreate(data, length);
  LCStringRef shaObj = LCStringCreate(sha);
  LCDictionarySetValueForKey(store->dict, shaObj, dataObj);
  LCRelease(dataObj);
  LCRelease(shaObj);
}

void memoryStoreDeleteData(void* storeObj, LCDataType type, char* sha) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCStringRef shaObj = LCStringCreate(sha);
  LCDictionaryDeleteKey(store->dict, shaObj);
  LCRelease(shaObj);
}

size_t memoryStoreGetDataLength(void* storeObj, LCDataType type, char* sha) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCStringRef shaObj = LCStringCreate(sha);
  LCDataRef data = LCDictionaryValueForKey(store->dict, shaObj);
  LCRelease(shaObj);
  if (data) {
    return LCDataLength(data);
  } else {
    return -1;
  }
}

void memoryStoreGetData(void* storeObj, LCDataType type, char* sha, unsigned char buffer[]) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCStringRef shaObj = LCStringCreate(sha);
  LCDataRef data = LCDictionaryValueForKey(store->dict, shaObj);
  LCRelease(shaObj);
  memcpy(buffer, LCDataDataRef(data), LCDataLength(data));
}