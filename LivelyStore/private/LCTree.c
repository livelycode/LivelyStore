
#include "LCStage.h"

LCDataArrayRef LCTreeDataArrayCopy(void* data);
void LCTreeDealloc(void* object);

struct LCTree {
  LCObjectInfo info;
  LCSHARef sha;
  size_t childTreesLength;
  size_t childDatasLength;
  LCKeyValueSHARef children[];
};

LCHashableObject hashableTree = {
  .dataArrayCopy = LCTreeDataArrayCopy,
};

LCType typeTree = {
  .dealloc = LCTreeDealloc,
  .meta = &hashableTree
};

LCTreeRef LCTreeCreate(LCKeyValueSHARef childTrees[], size_t childTreesLength,
                       LCKeyValueSHARef childDatas[], size_t childDatasLength) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree)+sizeof(LCKeyValueSHARef)*(childDatasLength+childTreesLength));
  if (newTree != NULL) {
    newTree->info.type = &typeTree;
    for (LCInteger i=0; i<childTreesLength; i++) {
      LCRetain(childTrees[i]);
    }
    for (LCInteger i=0; i<childDatasLength; i++) {
      LCRetain(childDatas[i]);
    }
    memcpy(newTree->children, childTrees, childTreesLength*sizeof(LCKeyValueSHARef));
    memcpy(&(newTree->children[childTreesLength]), childDatas, childDatasLength*sizeof(LCKeyValueSHARef));
    newTree->childTreesLength = childTreesLength;
    newTree->childDatasLength = childDatasLength;
  }
  return newTree;
};

size_t LCTreeChildTreesLength(LCTreeRef tree) {
  return tree->childTreesLength;
}

size_t LCTreeChildDatasLength(LCTreeRef tree) {
  return tree->childDatasLength;
}

LCKeyValueSHARef* LCTreeChildTrees(LCTreeRef tree) {
  return tree->children;
}

LCKeyValueSHARef* LCTreeChildDatas(LCTreeRef tree) {
  return &(tree->children[tree->childTreesLength]);
}

LCDataArrayRef LCTreeDataArrayCopy(void* data) {
  LCTreeRef tree = (LCTreeRef)data;
  size_t dataCount = tree->childTreesLength*2 + tree->childDatasLength*2;
  LCDataRef buffer[dataCount];
  LCInteger bufferIndex = 0;
  for (LCInteger i=0; i<dataCount/2; i++) {
    LCDataRef key = LCStringCreateData(LCKeyValueSHAKey(tree->children[i]));
    LCDataRef value = LCSHASHAData(LCKeyValueSHAValue(tree->children[i]));
    buffer[bufferIndex] = key;
    buffer[bufferIndex+1] = value;
    bufferIndex = bufferIndex+2;
  }
  return LCDataArrayCreate(buffer, dataCount);
}

LCSHARef LCTreeSHA(LCTreeRef tree) {
  if(tree->sha == NULL) {
    tree->sha = LCSHACreateFromHashableObject(tree);
  }
  return tree->sha;
}

void LCTreeDealloc(void* object) {
  LCTreeRef tree = (LCTreeRef)object;
  for (LCInteger i=0; i<(tree->childTreesLength+tree->childDatasLength); i++) {
    LCRelease(tree->children[i]);
  }
}