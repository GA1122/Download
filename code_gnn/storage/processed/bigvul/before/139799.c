views::NonClientFrameView* Shell::CreateDefaultNonClientFrameView(
    views::Widget* widget) {
  return new CustomFrameViewAsh(widget);
}
