ConstrainedWidthView::ConstrainedWidthView(views::View* child, int max_width)
    : max_width_(max_width) {
  SetLayoutManager(std::make_unique<views::FillLayout>());
  AddChildView(child);
}
