PopupHeaderView::PopupHeaderView(views::ButtonListener* close_button_listener)
    : name_(nullptr), status_(nullptr) {
  views::GridLayout* layout = new views::GridLayout(this);
  SetLayoutManager(layout);

  const int label_column = 0;
  views::ColumnSet* column_set = layout->AddColumnSet(label_column);
  column_set->AddPaddingColumn(0, kHeaderPaddingLeft);
  column_set->AddColumn(views::GridLayout::FILL,
                        views::GridLayout::FILL,
                        1,
                        views::GridLayout::USE_PREF,
                        0,
                        0);
  column_set->AddPaddingColumn(1, 0);
  column_set->AddColumn(views::GridLayout::FILL,
                        views::GridLayout::FILL,
                        1,
                        views::GridLayout::USE_PREF,
                        0,
                        0);
  column_set->AddPaddingColumn(0, kHeaderPaddingRight);

  layout->AddPaddingRow(0, kHeaderPaddingTop);

  layout->StartRow(0, label_column);
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  name_ = new views::Label(
      base::string16(), rb.GetFontList(ui::ResourceBundle::BoldFont));
  layout->AddView(name_, 1, 1, views::GridLayout::LEADING,
                  views::GridLayout::TRAILING);
  views::ImageButton* close_button =
      new views::ImageButton(close_button_listener);
  close_button->SetImage(views::CustomButton::STATE_NORMAL,
                         rb.GetImageNamed(IDR_CLOSE_2).ToImageSkia());
  close_button->SetImage(views::CustomButton::STATE_HOVERED,
                         rb.GetImageNamed(IDR_CLOSE_2_H).ToImageSkia());
  close_button->SetImage(views::CustomButton::STATE_PRESSED,
                         rb.GetImageNamed(IDR_CLOSE_2_P).ToImageSkia());
  layout->AddView(close_button, 1, 1, views::GridLayout::TRAILING,
                  views::GridLayout::LEADING);

  layout->AddPaddingRow(0, kHeaderRowSpacing);

  layout->StartRow(1, label_column);
  status_ = new views::Label(base::string16());
  status_->SetMultiLine(true);
  status_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  layout->AddView(status_,
                  1,
                  1,
                  views::GridLayout::LEADING,
                  views::GridLayout::LEADING);

  layout->AddPaddingRow(1, kHeaderPaddingBottom);
}
