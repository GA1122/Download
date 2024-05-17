void Layer::SetUserScrollable(bool horizontal, bool vertical) {
  DCHECK(IsPropertyChangeAllowed());
  if (user_scrollable_horizontal_ == horizontal &&
      user_scrollable_vertical_ == vertical)
    return;
  user_scrollable_horizontal_ = horizontal;
  user_scrollable_vertical_ = vertical;
  SetNeedsCommit();
}
