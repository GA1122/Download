static void ServerNameToSyncAPIName(const std::string& server_name,
                                    std::string* out) {
  CHECK(out);
  int length_to_copy = server_name.length();
  if (IsNameServerIllegalAfterTrimming(server_name) &&
      EndsWithSpace(server_name)) {
    --length_to_copy;
  }
  *out = std::string(server_name.c_str(), length_to_copy);
}
