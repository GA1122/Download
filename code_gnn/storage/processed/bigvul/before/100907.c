static DictionaryValue* CreateTaskGroupValue(const TaskManagerModel* tm,
                                            const int group_index) {
  DictionaryValue* val = new DictionaryValue();

  const int group_count = tm->GroupCount();
  if (group_index >= group_count)
     return val;

  int index = tm->GetResourceIndexForGroup(group_index, 0);
  std::pair<int, int> group_range;
  group_range = tm->GetGroupRangeForResource(index);
  int length = group_range.second;

  val->SetInteger("index", index);
  val->SetBoolean("isBackgroundResource",
                  tm->IsBackgroundResource(index));

  CreateGroupColumnList(tm, "type", index, 1, val);
  CreateGroupColumnList(tm, "processId", index, 1, val);
  CreateGroupColumnList(tm, "processIdValue", index, 1, val);
  CreateGroupColumnList(tm, "cpuUsage", index, 1, val);
  CreateGroupColumnList(tm, "cpuUsageValue", index, 1, val);
  CreateGroupColumnList(tm, "physicalMemory", index, 1, val);
  CreateGroupColumnList(tm, "physicalMemoryValue", index, 1, val);
  CreateGroupColumnList(tm, "sharedMemory", index, 1, val);
  CreateGroupColumnList(tm, "sharedMemoryValue", index, 1, val);
  CreateGroupColumnList(tm, "privateMemory", index, 1, val);
  CreateGroupColumnList(tm, "privateMemoryValue", index, 1, val);
  CreateGroupColumnList(tm, "webCoreImageCacheSize", index, 1, val);
  CreateGroupColumnList(tm, "webCoreImageCacheSizeValue", index, 1, val);
  CreateGroupColumnList(tm, "webCoreScriptsCacheSize", index, 1, val);
  CreateGroupColumnList(tm, "webCoreScriptsCacheSizeValue", index, 1, val);
  CreateGroupColumnList(tm, "webCoreCSSCacheSize", index, 1, val);
  CreateGroupColumnList(tm, "webCoreCSSCacheSizeValue", index, 1, val);
  CreateGroupColumnList(tm, "sqliteMemoryUsed", index, 1, val);
  CreateGroupColumnList(tm, "sqliteMemoryUsedValue", index, 1, val);
  CreateGroupColumnList(tm, "v8MemoryAllocatedSize", index, 1, val);
  CreateGroupColumnList(tm, "v8MemoryAllocatedSizeValue", index, 1, val);

  CreateGroupColumnList(tm, "uniqueId", index, length, val);
  CreateGroupColumnList(tm, "icon", index, length, val);
  CreateGroupColumnList(tm, "title", index, length, val);
  CreateGroupColumnList(tm, "profileName", index, length, val);
  CreateGroupColumnList(tm, "networkUsage", index, length, val);
  CreateGroupColumnList(tm, "networkUsageValue", index, length, val);
  CreateGroupColumnList(tm, "fps", index, length, val);
  CreateGroupColumnList(tm, "fpsValue", index, length, val);
  CreateGroupColumnList(tm, "goatsTeleported", index, length, val);
  CreateGroupColumnList(tm, "goatsTeleportedValue", index, length, val);
  CreateGroupColumnList(tm, "canInspect", index, length, val);
  CreateGroupColumnList(tm, "canActivate", index, length, val);

  return val;
}
