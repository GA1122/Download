bool DirectoryBackingStore::LoadEntries(MetahandlesIndex* entry_bucket) {
  string select;
  select.reserve(kUpdateStatementBufferSize);
  select.append("SELECT ");
  AppendColumnList(&select);
  select.append(" FROM metas ");
  sqlite_utils::SQLStatement statement;
  statement.prepare(load_dbhandle_, select.c_str());
  base::hash_set<int64> handles;
  EntryKernel* kernel = NULL;
  int query_result;
  while (SQLITE_ROW == (query_result = UnpackEntry(&statement, &kernel))) {
    DCHECK(handles.insert(kernel->ref(META_HANDLE)).second);   
    entry_bucket->insert(kernel);
  }
  return SQLITE_DONE == query_result;
}
