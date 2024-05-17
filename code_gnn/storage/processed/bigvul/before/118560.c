void GetSanitizedEnabledFlags(
    FlagsStorage* flags_storage, std::set<std::string>* result) {
  SanitizeList(flags_storage);
  *result = flags_storage->GetFlags();
}
