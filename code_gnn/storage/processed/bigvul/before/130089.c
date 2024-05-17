void GetExtensionAllowedTypesMap(
    ScopedVector<StringMappingListPolicyHandler::MappingEntry>* result) {
  for (size_t index = 0;
       index < extensions::schema_constants::kAllowedTypesMapSize;
       ++index) {
    const extensions::schema_constants::AllowedTypesMapEntry& entry =
        extensions::schema_constants::kAllowedTypesMap[index];
    result->push_back(new StringMappingListPolicyHandler::MappingEntry(
        entry.name,
        scoped_ptr<base::Value>(
            new base::FundamentalValue(entry.manifest_type))));
  }
}
