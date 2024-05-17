AutofillDialogViews::NotificationArea::NotificationArea(
    AutofillDialogViewDelegate* delegate)
    : delegate_(delegate) {
  SetBorder(views::Border::CreateEmptyBorder(kArrowHeight - 1, 0, 0, 0));

  views::BoxLayout* box_layout =
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 0);
  SetLayoutManager(box_layout);
}
