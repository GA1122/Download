  BackgroundColorGroupTableView(TaskManagerTableModel* model,
                                const std::vector<ui::TableColumn>& columns,
                                bool highlight_background_resources)
      : views::GroupTableView(model, columns, views::ICON_AND_TEXT,
                              false, true, true, true),
        model_(model) {
    SetCustomColorsEnabled(highlight_background_resources);
  }
