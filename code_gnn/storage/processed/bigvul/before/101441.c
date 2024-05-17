int DirectoryBackingStore::CreateMetasTable(bool is_temporary) {
  const char* name = is_temporary ? "temp_metas" : "metas";
  string query = "CREATE TABLE ";
  query.append(name);
  query.append(ComposeCreateTableColumnSpecs());
  return ExecQuery(load_dbhandle_, query.c_str());
}
