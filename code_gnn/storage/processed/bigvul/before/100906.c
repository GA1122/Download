static Value* CreateColumnValue(const TaskManagerModel* tm,
                                const std::string column_name,
                                const int i) {
  if (column_name == "uniqueId")
    return Value::CreateIntegerValue(tm->GetResourceUniqueId(i));
  if (column_name == "type")
    return Value::CreateStringValue(
        TaskManager::Resource::GetResourceTypeAsString(
        tm->GetResourceType(i)));
  if (column_name == "processId")
    return Value::CreateStringValue(tm->GetResourceProcessId(i));
  if (column_name == "processIdValue")
    return Value::CreateIntegerValue(tm->GetProcessId(i));
  if (column_name == "cpuUsage")
    return Value::CreateStringValue(tm->GetResourceCPUUsage(i));
  if (column_name == "cpuUsageValue")
    return Value::CreateDoubleValue(tm->GetCPUUsage(i));
  if (column_name == "privateMemory")
    return Value::CreateStringValue(tm->GetResourcePrivateMemory(i));
  if (column_name == "privateMemoryValue") {
    size_t private_memory;
    tm->GetPrivateMemory(i, &private_memory);
    return Value::CreateDoubleValue(private_memory);
  }
  if (column_name == "sharedMemory")
    return Value::CreateStringValue(tm->GetResourceSharedMemory(i));
  if (column_name == "sharedMemoryValue") {
    size_t shared_memory;
    tm->GetSharedMemory(i, &shared_memory);
    return Value::CreateDoubleValue(shared_memory);
  }
  if (column_name == "physicalMemory")
    return Value::CreateStringValue(tm->GetResourcePhysicalMemory(i));
  if (column_name == "physicalMemoryValue") {
    size_t physical_memory;
    tm->GetPhysicalMemory(i, &physical_memory);
    return Value::CreateDoubleValue(physical_memory);
  }
  if (column_name == "icon")
    return Value::CreateStringValue(
               web_ui_util::GetImageDataUrl(tm->GetResourceIcon(i)));
  if (column_name == "title")
    return Value::CreateStringValue(tm->GetResourceTitle(i));
  if (column_name == "profileName")
    return Value::CreateStringValue(tm->GetResourceProfileName(i));
  if (column_name == "networkUsage")
    return Value::CreateStringValue(tm->GetResourceNetworkUsage(i));
  if (column_name == "networkUsageValue")
    return Value::CreateDoubleValue(tm->GetNetworkUsage(i));
  if (column_name == "webCoreImageCacheSize")
    return Value::CreateStringValue(tm->GetResourceWebCoreImageCacheSize(i));
  if (column_name == "webCoreImageCacheSizeValue") {
    WebKit::WebCache::ResourceTypeStats resource_stats;
    tm->GetWebCoreCacheStats(i, &resource_stats);
    return Value::CreateDoubleValue(resource_stats.images.size);
  }
  if (column_name == "webCoreScriptsCacheSize")
    return Value::CreateStringValue(tm->GetResourceWebCoreScriptsCacheSize(i));
  if (column_name == "webCoreScriptsCacheSizeValue") {
    WebKit::WebCache::ResourceTypeStats resource_stats;
    tm->GetWebCoreCacheStats(i, &resource_stats);
    return Value::CreateDoubleValue(resource_stats.scripts.size);
  }
  if (column_name == "webCoreCSSCacheSize")
    return Value::CreateStringValue(tm->GetResourceWebCoreCSSCacheSize(i));
  if (column_name == "webCoreCSSCacheSizeValue") {
    WebKit::WebCache::ResourceTypeStats resource_stats;
    tm->GetWebCoreCacheStats(i, &resource_stats);
    return Value::CreateDoubleValue(resource_stats.cssStyleSheets.size);
  }
  if (column_name == "fps")
    return Value::CreateStringValue(tm->GetResourceFPS(i));
  if (column_name == "fpsValue") {
    float fps;
    tm->GetFPS(i, &fps);
    return Value::CreateDoubleValue(fps);
  }
  if (column_name == "sqliteMemoryUsed")
    return Value::CreateStringValue(tm->GetResourceSqliteMemoryUsed(i));
  if (column_name == "sqliteMemoryUsedValue") {
    size_t sqlite_memory;
    tm->GetSqliteMemoryUsedBytes(i, &sqlite_memory);
    return Value::CreateDoubleValue(sqlite_memory);
  }
  if (column_name == "goatsTeleported")
    return Value::CreateStringValue(tm->GetResourceGoatsTeleported(i));
  if (column_name == "goatsTeleportedValue")
    return Value::CreateIntegerValue(tm->GetGoatsTeleported(i));
  if (column_name == "v8MemoryAllocatedSize")
    return Value::CreateStringValue(tm->GetResourceV8MemoryAllocatedSize(i));
  if (column_name == "v8MemoryAllocatedSizeValue") {
    size_t v8_memory;
    tm->GetV8Memory(i, &v8_memory);
    return Value::CreateDoubleValue(v8_memory);
  }
  if (column_name == "canInspect")
    return Value::CreateBooleanValue(tm->CanInspect(i));
  if (column_name == "canActivate")
    return Value::CreateBooleanValue(tm->CanActivate(i));

  NOTREACHED();
  return NULL;
 }
