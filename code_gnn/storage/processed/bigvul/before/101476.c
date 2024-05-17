bool DirectoryBackingStore::SaveEntryToDB(const EntryKernel& entry) {
  DCHECK(save_dbhandle_);
  string query;
  query.reserve(kUpdateStatementBufferSize);
  query.append("INSERT OR REPLACE INTO metas ");
  string values;
  values.reserve(kUpdateStatementBufferSize);
  values.append("VALUES ");
  const char* separator = "( ";
  int i = 0;
  for (i = BEGIN_FIELDS; i < PROTO_FIELDS_END; ++i) {
    query.append(separator);
    values.append(separator);
    separator = ", ";
    query.append(ColumnName(i));
    values.append("?");
  }

  query.append(" ) ");
  values.append(" )");
  query.append(values);
  sqlite_utils::SQLStatement statement;
  statement.prepare(save_dbhandle_, query.c_str());
  BindFields(entry, &statement);
  return (SQLITE_DONE == statement.step() &&
          SQLITE_OK == statement.reset() &&
          1 == statement.changes());
}
