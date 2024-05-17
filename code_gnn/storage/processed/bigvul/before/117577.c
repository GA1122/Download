void SettingLevelBubbleDelegateView::Init() {
  SetLayoutManager(new views::FillLayout());
  view_ = new SettingLevelBubbleView();
  AddChildView(view_);
}
