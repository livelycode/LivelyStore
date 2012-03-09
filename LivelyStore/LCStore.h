

#ifndef LivelyStore_LCStore_h
#define LivelyStore_LCStore_h

#include "LivelyStore.h"

LCStoreRef LCStoreCreate(struct LCStoreBackend* backend, char headCommit[LC_SHA1_HEX_Length]);
void LCStorePull(LCStoreRef target, LCStoreRef source);
void LCStorePush(LCStoreRef source, LCStoreRef target);
void LCStoreCommit(LCStoreRef store, LCStageRef stage);
void LCStoreHead(LCStoreRef store, char* headShaBuffer);
LCSuccess LCStoreDataSHA(LCStoreRef store, char* commit, char* path, char dataSHABuffer[LC_SHA1_HEX_Length]);
size_t LCStoreDataLength(LCStoreRef store, char sha[LC_SHA1_HEX_Length]);
LCSuccess LCStoreData(LCStoreRef store, char sha[LC_SHA1_HEX_Length], unsigned char dataBuffer[]);
#endif
