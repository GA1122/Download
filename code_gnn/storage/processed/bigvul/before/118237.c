gfx::Size AutofillDialogViews::NotificationArea::GetPreferredSize() const {
  gfx::Size size = views::View::GetPreferredSize();
  size.set_width(1);
  return size;
}
