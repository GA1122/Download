string ComposeCreateTableColumnSpecs() {
  const ColumnSpec* begin = g_metas_columns;
  const ColumnSpec* end = g_metas_columns + arraysize(g_metas_columns);
  string query;
  query.reserve(kUpdateStatementBufferSize);
  char separator = '(';
  for (const ColumnSpec* column = begin; column != end; ++column) {
    query.push_back(separator);
    separator = ',';
    query.append(column->name);
    query.push_back(' ');
    query.append(column->spec);
  }
  query.push_back(')');
  return query;
}
