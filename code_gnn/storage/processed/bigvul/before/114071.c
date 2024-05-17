void TaskManagerView::UpdateStatsCounters() {
  base::StatsTable* stats = base::StatsTable::current();
  if (stats != NULL) {
    int max = stats->GetMaxCounters();
    for (int i = 1; i < max; i++) {
      const char* row = stats->GetRowName(i);
      if (row != NULL && row[0] != '\0' && !tab_table_->HasColumn(i)) {
        ui::TableColumn col(i, ASCIIToUTF16(row), ui::TableColumn::RIGHT, 90,
                            0);
        col.sortable = true;
        columns_.push_back(col);
        tab_table_->AddColumn(col);
      }
    }
  }
}
