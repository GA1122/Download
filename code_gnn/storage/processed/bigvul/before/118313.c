AutofillDialogViews::SuggestionView::SuggestionView(
    AutofillDialogViews* autofill_dialog)
    : label_(new views::Label()),
      label_line_2_(new views::Label()),
      icon_(new views::ImageView()),
      textfield_(
          new ExpandingTextfield(base::string16(),
                                 base::string16(),
                                 false,
                                 autofill_dialog)) {
  SetBorder(views::Border::CreateSolidSidedBorder(1, 0, 0, 0, SK_ColorLTGRAY));

  SectionRowView* label_container = new SectionRowView();
  AddChildView(label_container);

  label_container->AddChildView(icon_);
  label_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  label_container->AddChildView(label_);

  textfield_->SetVisible(false);
  textfield_->SetDefaultWidthInCharacters(15);
  label_container->AddChildView(textfield_);

  label_line_2_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  label_line_2_->SetVisible(false);
  label_line_2_->SetLineHeight(22);
  label_line_2_->SetMultiLine(true);
  AddChildView(label_line_2_);

  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 7));
}
