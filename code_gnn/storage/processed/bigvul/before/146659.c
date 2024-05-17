void BubbleHeaderView::AddResetDecisionsLabel() {
  std::vector<base::string16> subst;
  subst.push_back(
      l10n_util::GetStringUTF16(IDS_PAGE_INFO_INVALID_CERTIFICATE_DESCRIPTION));
  subst.push_back(l10n_util::GetStringUTF16(
      IDS_PAGE_INFO_RESET_INVALID_CERTIFICATE_DECISIONS_BUTTON));

  std::vector<size_t> offsets;

  base::string16 text = base::ReplaceStringPlaceholders(
      base::ASCIIToUTF16("$1 $2"), subst, &offsets);
  reset_cert_decisions_label_ =
      new views::StyledLabel(text, styled_label_listener_);
  reset_cert_decisions_label_->set_id(
      PageInfoBubbleView::VIEW_ID_PAGE_INFO_LABEL_RESET_CERTIFICATE_DECISIONS);
  gfx::Range link_range(offsets[1], text.length());

  views::StyledLabel::RangeStyleInfo link_style =
      views::StyledLabel::RangeStyleInfo::CreateForLink();
  link_style.disable_line_wrapping = false;

  reset_cert_decisions_label_->AddStyleRange(link_range, link_style);
  reset_cert_decisions_label_->SizeToFit(0);
  reset_decisions_label_container_->AddChildView(reset_cert_decisions_label_);

  reset_decisions_label_container_->SetBorder(
      views::CreateEmptyBorder(8, 0, 0, 0));

  InvalidateLayout();
}
