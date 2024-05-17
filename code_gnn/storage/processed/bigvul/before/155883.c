views::GridLayout* CreateSingleColumnLayout(views::View* view, int width) {
  views::GridLayout* layout =
      view->SetLayoutManager(std::make_unique<views::GridLayout>(view));

  views::ColumnSet* columns = layout->AddColumnSet(0);
  columns->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
                     views::GridLayout::kFixedSize, views::GridLayout::FIXED,
                     width, width);
  return layout;
}
