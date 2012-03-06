
#ifndef LivelyStore_LCDataStore_h
#define LivelyStore_LCDataStore_h

#include "LivelyStore.h"

LCDataStoreRef LCDataStoreCreate(LCStringRef location);
void LCDataStoreSetStoreObject(LCDataStoreRef store, void* storeObject);
void LCDataStoreSetNewDataCallback(LCDataStoreRef store, LCStoreDataCb callback);
void LCDataStoreSetDeletedDataCallback(LCDataStoreRef store, LCDeleteDataCb callback);
void LCDataStoreSetGetDataCallback(LCDataStoreRef store, LCGetDataCb callback);
void LCDataStoreSetGetDataLengthCallback(LCDataStoreRef store, LCGetDataLengthCb callback);

void LCDataStorePutData(LCDataStoreRef store, LCStringRef sha, LCDataRef data);
void LCDataStoreDeleteData(LCDataStoreRef store, LCStringRef sha);
void LCDataStorePutTreeData(LCDataStoreRef store, LCStringRef sha, LCStringRef data);
void LCDataStorePutCommitData(LCDataStoreRef store, LCStringRef sha, LCStringRef data);
void LCDataStorePutTagData(LCDataStoreRef store, LCStringRef tag, LCStringRef data);
LCStringRef LCDataStoreGetTreeData(LCDataStoreRef store, LCStringRef sha);

#endif
