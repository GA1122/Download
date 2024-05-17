views::GridLayout* ExtensionInstallDialogView::CreateLayout(
    int left_column_width,
    int column_set_id) {
  container_ = new views::View();
  views::GridLayout* layout = new views::GridLayout(container_);
  layout->SetInsets(0, views::kButtonHEdgeMarginNew, views::kPanelVertMargin,
                    0);
  container_->SetLayoutManager(layout);
  AddChildView(container_);

  views::ColumnSet* column_set = layout->AddColumnSet(column_set_id);
  column_set->AddColumn(views::GridLayout::LEADING, views::GridLayout::LEADING,
                        0,   
                        views::GridLayout::USE_PREF,
                        0,   
                        left_column_width);
  column_set->AddPaddingColumn(0, views::kPanelHorizMargin);
  column_set->AddColumn(views::GridLayout::TRAILING, views::GridLayout::LEADING,
                        0,   
                        views::GridLayout::USE_PREF,
                        0,   
                        kIconSize);
  column_set->AddPaddingColumn(0, views::kButtonHEdgeMarginNew);

  layout->StartRow(0, column_set_id);
  views::Label* title =
      new views::Label(prompt_->GetDialogTitle(),
                       ui::ResourceBundle::GetSharedInstance().GetFontList(
                           ui::ResourceBundle::MediumFont));
  title->SetMultiLine(true);
  title->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  title->SizeToFit(left_column_width);
  layout->AddView(title);

  const gfx::ImageSkia* image = prompt_->icon().ToImageSkia();
  gfx::Size size(image->width(), image->height());
  if (size.width() > kIconSize || size.height() > kIconSize)
    size = gfx::Size(kIconSize, kIconSize);
  views::ImageView* icon = new views::ImageView();
  icon->SetImageSize(size);
  icon->SetImage(*image);

  int icon_row_span = 1;   
  if (prompt_->has_webstore_data()) {
    icon_row_span += 3;
  } else {
    icon_row_span += 1;
  }
  layout->AddView(icon, 1, icon_row_span);

  return layout;
}
