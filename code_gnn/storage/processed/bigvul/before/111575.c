  GDataEntry* FindEntryByResourceId(const std::string& resource_id) {
    GDataEntry* entry = NULL;
    file_system_->FindEntryByResourceIdSync(
        resource_id, base::Bind(&ReadOnlyFindEntryCallback, &entry));
    return entry;
  }
