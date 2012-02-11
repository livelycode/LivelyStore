
#include "LivelyStoreTests.h"
#include "minuit.h"

int tests_run = 0;

static char* test_retain_counting() {
  LCStringRef test = LCStringCreate("abc");
  mu_assert("initial retain count=0", LCRetainCount(test)==0);
  LCRetain(test);
  mu_assert("retain increases retain count", LCRetainCount(test)==1);
  LCRelease(test);
  mu_assert("releasing decreases retain count", LCRetainCount(test)==0);
  LCRelease(test);
  return 0;
}

static char* test_string() {
  char* aCString = "abcd";
  char* anIdenticalCString = "abcd";
  LCStringRef aLCString = LCStringCreate(aCString);
  LCStringRef anIdenticalLCString = LCStringCreate(anIdenticalCString);
  
  mu_assert("LCStringEqual is correct", LCStringEqual(aLCString, anIdenticalLCString));
  
  LCStringRef bothStrings = LCStringCreate("abcdabcd");
  LCStringRef stringArray[] = {aLCString, anIdenticalLCString};
  LCStringRef mergedString = LCStringCreateFromStrings(stringArray, 2);
  mu_assert("LCStringCreateFromStrings is correct", LCStringEqual(mergedString, bothStrings));
  return 0;
}

static char* test_dataArray() {
  char* string1 = "abc";
  char* string2 = "def";
  
  LCDataRef data1 = LCDataCreate((LCByte*)string1, strlen(string1)+1);
  LCDataRef data2 = LCDataCreate((LCByte*)string2, strlen(string2)+1);
  LCDataRef datas[] = {data1, data2};
  LCDataArrayRef array = LCDataArrayCreate(datas, 2);
  LCBool correct = (LCDataDataAtIndex(array, 0) == data1) && (LCDataDataAtIndex(array, 1) == data2);
  mu_assert("LCDataArray stores elements correctly", correct);
  return 0;
}

static char* test_sha1() {
  char* testData1 = "compute sha1";
  char* testData2 = " abc";
  //char* testData3 = "compute sha1 abc";
  LCStringRef testData1RealSHA = LCStringCreate("a0e1e14cb346a01f9fd10e60080b945f995524af");
  LCStringRef testData3RealSHA = LCStringCreate("253f7e747aa2bd4107cc0af2f78fe60fc075d090");
  
  LCDataRef testData1Data = LCStringCreateData(LCStringCreate(testData1));
  LCDataRef testData1DataNoNull = LCDataCreate((LCByte*)testData1, strlen(testData1));
  LCDataRef testData2Data = LCStringCreateData(LCStringCreate(testData2));
  LCDataRef testData1and2Datas[] = {testData1DataNoNull, testData2Data};
  LCDataArrayRef testData1and2DataArray = LCDataArrayCreate(testData1and2Datas, 2);
  
  LCSHARef testData1SHA = LCSHACreateFromHashableObject(testData1Data);
  LCSHARef testData1SHAClone = LCDataSHA1(testData1Data);
  LCSHARef testData1and2SHA = LCSHACreateFromHashableObject(testData1and2DataArray);
  
  mu_assert("SHA1 from testData1 is correct", LCStringEqual(LCSHACreateHexString(testData1SHA), testData1RealSHA));
  mu_assert("SHA1 from two LCDatas is correct", LCStringEqual(LCSHACreateHexString(testData1and2SHA), testData3RealSHA));
  mu_assert("LCSHAEqual is correct", LCSHAEqual(testData1SHA, testData1SHAClone));
  
  LCSHARef fromHexString = LCSHACreateFromHexString(testData1RealSHA, NULL);
  LCStringRef hexStringAgain = LCSHACreateHexString(fromHexString);
  mu_assert("LCSHA can be created from hex string", LCStringEqual(hexStringAgain, testData1RealSHA));
  return 0;
}


static char* test_data() {
  char* aCString = "normal string";
  char* sameCString = "normal string";
  char* differentCString = "different string";

  LCDataRef aData = LCDataCreate((LCByte*)aCString, strlen(aCString)+1);
  LCDataRef sameData = LCDataCreate((LCByte*)sameCString, strlen(sameCString)+1);
  LCDataRef differentData = LCDataCreate((LCByte*)differentCString, strlen(differentCString)+1);

  
  LCByte dataData[LCDataLength(aData)];
  LCDataData(aData, dataData);
  
  mu_assert("LCData stores data correctly", strcmp(aCString, (char*)dataData)==0);
  mu_assert("LCDataEqual works with same data data", LCDataEqual(aData, sameData));
  mu_assert("LCDataEqual works with differing data data", LCDataEqual(aData, differentData)==false);
  LCStringRef aLCString = LCStringCreate(aCString);
  LCStringRef stringFromData = LCBLobCreateString(aData);
  mu_assert("LCData returns correct string", LCStringEqual(stringFromData, aLCString));
  return 0;
}

static char* test_keyValue() {
  LCStringRef someKey = LCStringCreate("somekey");
  LCDataRef someValue = LCDataCreate((LCByte*)"12345", 6);
  LCKeyValueRef keyValue = LCKeyValueCreate(someKey, someValue);
  mu_assert("LCKeyValue stores key correctly", LCStringEqual(LCKeyValueKey(keyValue), someKey));
  mu_assert("LCKeyValue stores value correctly", LCStringEqual(LCKeyValueKey(keyValue), someKey));
  return 0;
}

static char* test_commit() {
  LCStringRef key1 = LCStringCreate("key1");
  LCStringRef key2 = LCStringCreate("key2");
  LCDataRef value1 = LCDataCreate((LCByte*)"12345", 6);
  LCDataRef value2 = LCDataCreate((LCByte*)"67890", 6);
  LCKeyValueRef entry1 = LCKeyValueCreate(key1, value1);
  LCKeyValueRef entry2 = LCKeyValueCreate(key2, value2);
  
  LCStageRef commit = LCStageCreate();
  LCStageAddEntry(commit, entry1);
  LCStageAddEntry(commit, entry2);

  mu_assert("LCStage entry adding works", LCStageEntryCount(commit)==2);
  
  LCKeyValueRef entries[LCStageEntryCount(commit)];
  LCStageEntries(commit, entries);
  mu_assert("LCStage stores entries correctly", (entries[0] == entry1) && (entries[1] == entry2));
  return 0;
}

static char* test_tree() {
  LCStringRef key1 = LCStringCreate("key1");
  LCStringRef key2 = LCStringCreate("key2");
  LCStringRef key3 = LCStringCreate("key3");
  LCSHARef value1SHA = LCDataSHA1(LCDataCreate((LCByte*)"12345", 6));
  LCSHARef value2SHA = LCDataSHA1(LCDataCreate((LCByte*)"67890", 6));
  LCKeyValueSHARef entry1 = LCKeyValueSHACreate(key1, value1SHA);
  LCKeyValueSHARef entry2 = LCKeyValueSHACreate(key2, value2SHA);
  LCKeyValueSHARef entry3 = LCKeyValueSHACreate(key3, value2SHA);
  
  LCKeyValueSHARef datas1[] = {entry1};
  LCKeyValueSHARef datas2[] = {entry2, entry3};
  
  LCStringRef tree1Name = LCStringCreate("");
  LCTreeRef tree1 = LCTreeCreate(NULL, 0, datas1, 1);
  LCKeyValueSHARef childTree1 = LCKeyValueSHACreate(tree1Name, LCTreeSHA(tree1));
  LCKeyValueSHARef childTrees1[] = {childTree1};
  LCTreeRef tree2 = LCTreeCreate(childTrees1, 1, datas2, 2);
  
  LCKeyValueSHARef* tree1Datas = LCTreeChildDatas(tree1);
  LCKeyValueSHARef* tree2Datas = LCTreeChildDatas(tree2);
  LCKeyValueSHARef* tree2ChildTrees = LCTreeChildTrees(tree2);
  
  LCBool correct = (tree1Datas[0]==entry1) && (tree2Datas[0]==entry2) && (tree2Datas[1]==entry3);
  mu_assert("LCTree stores entries correctly", correct);
  mu_assert("LCTree stores child trees correctly", tree2ChildTrees[0] == childTree1);
  
  LCTreeRef tree2Clone = LCTreeCreate(childTrees1, 1, datas2, 2);
  LCSHARef tree1SHA = LCTreeSHA(tree1);
  LCSHARef tree2SHA = LCTreeSHA(tree2);
  LCSHARef tree2CloneSHA = LCTreeSHA(tree2Clone);
  mu_assert("LCTree SHA differs on differing trees", LCSHAEqual(tree1SHA, tree2SHA)==false);
  mu_assert("LCTree SHA is identical on identical trees", LCSHAEqual(tree2SHA, tree2CloneSHA));
  return 0;
}

static char* all_tests() {
  mu_run_test(test_retain_counting);
  mu_run_test(test_string);
  mu_run_test(test_data);
  mu_run_test(test_dataArray);
  mu_run_test(test_sha1);
  mu_run_test(test_keyValue);
  mu_run_test(test_commit);
  mu_run_test(test_tree);
  return 0;
}

LCBool testsRun() {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  
  return result == 0;
}
