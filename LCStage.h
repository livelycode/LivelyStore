

#ifndef LivelyStore_LCStage_h
#define LivelyStore_LCStage_h

#include "LivelyStore.h"

LCStageRef LCStageCreate();
void LCStageAddEntry(LCStageRef stage, char* path, unsigned char data[], size_t length);
void LCStageDeletePath(LCStageRef stage, char* path);

// private
LCKeyValueRef* LCStagePathsToAdd(LCStageRef stage);
LCStringRef* LCStagePathsToDelete(LCStageRef stage);
size_t LCStageAddPathsCount(LCStageRef stage);
size_t LCStageDeletePathsCount(LCStageRef stage);

#endif