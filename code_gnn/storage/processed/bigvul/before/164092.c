bool CreateTable(sql::Database* db, const TableInfo& info) {
  std::string sql("CREATE TABLE ");
  sql += info.table_name;
  sql += info.columns;
  return db->Execute(sql.c_str());
}
