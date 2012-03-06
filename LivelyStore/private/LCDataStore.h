
#ifndef LivelyStore_LCDataStore_h
#define LivelyStore_LCDataStore_h

#include "LivelyStore.h"

LCDataStoreRef LCDataStoreCreate(LCStringRef location);
void LCDataStoreSetNewDataCallback(LCDataStoreRef store, LCStoreDataCb callback);
void LCDataStoreSetDeletedDataCallback(LCDataStoreRef store, LCDeleteDataCb callback);
void LCDataStoreSetGetDataCallback(LCDataStoreRef store, LCGetDataCb callback);

void LCDataStorePutData(LCDataStoreRef store, LCStringRef sha, LCByte data[], size_t length);
void LCDataStoreDeleteData(LCDataStoreRef store, LCStringRef sha);
void LCDataStorePutTreeData(LCDataStoreRef store, LCStringRef sha, LCStringRef data);
void LCDataStorePutCommitData(LCDataStoreRef store, LCStringRef sha, LCStringRef data);
void LCDataStorePutTagData(LCDataStoreRef store, LCStringRef tag, LCStringRef data);

#endif
