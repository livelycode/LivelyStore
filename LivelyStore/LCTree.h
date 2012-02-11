

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCStringRef name, LCTreeRef childTrees[], size_t childTreesLength,
                       LCKeyValueRef childEntries[], size_t childEntriesLength);
LCStringRef LCTreeName(LCTreeRef tree);
size_t LCTreeChildTreesLength(LCTreeRef tree);
size_t LCTreeChildEntriesLength(LCTreeRef tree);
LCTreeRef* LCTreeChildTrees(LCTreeRef tree);
LCKeyValueRef* LCTreeChildEntries(LCTreeRef tree);
LCSHARef LCTreeSHA1(LCTreeRef tree);

#endif
