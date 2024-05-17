ExpandableContainerView::DetailsView::DetailsView(int horizontal_space,
                                                  bool parent_bulleted)
    : layout_(new views::GridLayout(this)),
      state_(0) {
  SetLayoutManager(layout_);
  views::ColumnSet* column_set = layout_->AddColumnSet(0);
  const int padding = GetLeftPaddingForBulletedItems(parent_bulleted);
  column_set->AddPaddingColumn(0, padding);
  column_set->AddColumn(views::GridLayout::LEADING, views::GridLayout::LEADING,
                        0, views::GridLayout::FIXED, horizontal_space - padding,
                        0);
}
