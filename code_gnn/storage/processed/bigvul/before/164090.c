bool CreateIndex(sql::Database* db, const IndexInfo& info) {
  std::string sql;
  if (info.unique)
    sql += "CREATE UNIQUE INDEX ";
  else
    sql += "CREATE INDEX ";
  sql += info.index_name;
  sql += " ON ";
  sql += info.table_name;
  sql += info.columns;
  return db->Execute(sql.c_str());
}
