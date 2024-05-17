void ExpandableContainerView::DetailsView::AddDetail(
    const base::string16& detail) {
  layout_->StartRowWithPadding(0, 0,
                               0, views::kRelatedControlSmallVerticalSpacing);
  views::Label* detail_label =
      new views::Label(PrepareForDisplay(detail, false));
  detail_label->SetMultiLine(true);
  detail_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  layout_->AddView(detail_label);
}
