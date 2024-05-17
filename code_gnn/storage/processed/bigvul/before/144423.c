IconedView::IconedView(views::View* view, const gfx::ImageSkia& image) {
  views::GridLayout* layout = new views::GridLayout(this);
  SetLayoutManager(layout);
  views::ColumnSet* column_set = layout->AddColumnSet(0);
  column_set->AddColumn(views::GridLayout::CENTER,
                        views::GridLayout::LEADING,
                        0,
                        views::GridLayout::FIXED,
                        kSmallIconSize,
                        0);
  column_set->AddPaddingColumn(0, kSmallIconPadding);
  column_set->AddColumn(views::GridLayout::LEADING,
                        views::GridLayout::CENTER,
                        0,
                        views::GridLayout::USE_PREF,
                        0,   
                        0);
  layout->StartRow(0, 0);

  gfx::Size size(image.width(), image.height());
  if (size.width() > kSmallIconSize || size.height() > kSmallIconSize)
    size = gfx::Size(kSmallIconSize, kSmallIconSize);
  views::ImageView* image_view = new views::ImageView;
  image_view->SetImage(image);
  image_view->SetImageSize(size);

  layout->AddView(image_view);
  layout->AddView(view);
}
