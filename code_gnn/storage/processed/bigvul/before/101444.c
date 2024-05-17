int DirectoryBackingStore::CreateShareInfoTableVersion71(
    bool is_temporary) {
  const char* name = is_temporary ? "temp_share_info" : "share_info";
  string query = "CREATE TABLE ";
  query.append(name);
  query.append(" ("
      "id TEXT primary key, "
      "name TEXT, "
      "store_birthday TEXT, "
      "db_create_version TEXT, "
      "db_create_time INT, "
      "next_id INT default -2, "
      "cache_guid TEXT )");
  return ExecQuery(load_dbhandle_, query.c_str());
}
