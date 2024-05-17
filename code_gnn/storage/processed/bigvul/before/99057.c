void PageInfoWindowView::LayoutSections() {
  RemoveAllChildViews(true);

  views::GridLayout* layout = new views::GridLayout(this);
  SetLayoutManager(layout);
  views::ColumnSet* columns = layout->AddColumnSet(0);
  columns->AddPaddingColumn(0, kHorizontalPadding);
  columns->AddColumn(views::GridLayout::FILL,   
                     views::GridLayout::FILL,   
                     1,   
                     views::GridLayout::USE_PREF,   
                     0,   
                     0);   
  columns->AddPaddingColumn(0, kHorizontalPadding);

  layout->AddPaddingRow(0, kVerticalPadding);
  for (int i = 0; i < model_.GetSectionCount(); ++i) {
    PageInfoModel::SectionInfo info = model_.GetSectionInfo(i);
    layout->StartRow(0, 0);
    layout->AddView(new Section(info.title, info.state, info.head_line,
                                info.description));
    layout->AddPaddingRow(0, kVerticalPadding);
  }
  layout->AddPaddingRow(1, kVerticalPadding);

  Layout();
}
