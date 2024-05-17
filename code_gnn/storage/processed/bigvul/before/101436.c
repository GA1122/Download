bool DirectoryBackingStore::AddColumn(const ColumnSpec* column) {
  sqlite_utils::SQLStatement add_column;
  std::string sql = base::StringPrintf(
      "ALTER TABLE metas ADD COLUMN %s %s", column->name, column->spec);
  add_column.prepare(load_dbhandle_, sql.c_str());
  return add_column.step() == SQLITE_DONE;
}
