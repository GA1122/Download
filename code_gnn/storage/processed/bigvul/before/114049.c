string16 TaskManagerTableModel::GetText(int row, int col_id) {
  switch (col_id) {
    case IDS_TASK_MANAGER_TASK_COLUMN:   
      return model_->GetResourceTitle(row);

    case IDS_TASK_MANAGER_PROFILE_NAME_COLUMN:   
      return model_->GetResourceProfileName(row);

    case IDS_TASK_MANAGER_NET_COLUMN:   
      return model_->GetResourceNetworkUsage(row);

    case IDS_TASK_MANAGER_CPU_COLUMN:   
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceCPUUsage(row);

    case IDS_TASK_MANAGER_PRIVATE_MEM_COLUMN:   
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourcePrivateMemory(row);

    case IDS_TASK_MANAGER_SHARED_MEM_COLUMN:   
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceSharedMemory(row);

    case IDS_TASK_MANAGER_PHYSICAL_MEM_COLUMN:   
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourcePhysicalMemory(row);

    case IDS_TASK_MANAGER_PROCESS_ID_COLUMN:
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceProcessId(row);

    case IDS_TASK_MANAGER_GOATS_TELEPORTED_COLUMN:   
      return model_->GetResourceGoatsTeleported(row);

    case IDS_TASK_MANAGER_WEBCORE_IMAGE_CACHE_COLUMN:
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceWebCoreImageCacheSize(row);

    case IDS_TASK_MANAGER_WEBCORE_SCRIPTS_CACHE_COLUMN:
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceWebCoreScriptsCacheSize(row);

    case IDS_TASK_MANAGER_WEBCORE_CSS_CACHE_COLUMN:
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceWebCoreCSSCacheSize(row);

    case IDS_TASK_MANAGER_FPS_COLUMN:
      return model_->GetResourceFPS(row);

    case IDS_TASK_MANAGER_SQLITE_MEMORY_USED_COLUMN:
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceSqliteMemoryUsed(row);

    case IDS_TASK_MANAGER_JAVASCRIPT_MEMORY_ALLOCATED_COLUMN:
      if (!model_->IsResourceFirstInGroup(row))
        return string16();
      return model_->GetResourceV8MemoryAllocatedSize(row);

    default:
      NOTREACHED();
      return string16();
  }
}
