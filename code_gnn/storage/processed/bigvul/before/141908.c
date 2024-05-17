void AutofillPopupSeparatorView::CreateContent() {
  SetLayoutManager(std::make_unique<views::FillLayout>());

  views::Separator* separator = new views::Separator();
  separator->SetColor(popup_view_->GetSeparatorColor());
  separator->SetPreferredHeight(
      views::MenuConfig::instance().separator_thickness);
  separator->SetBorder(views::CreateEmptyBorder(
       GetContentsVerticalPadding(),
       0,
       0,
       0));
  AddChildView(separator);

  SetBackground(CreateBackground());
}
