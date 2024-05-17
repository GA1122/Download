int MaxStorageSizeForTable(int table_len) {
  return table_len * (k64kEntriesStore / kBaseTableLen);
}
