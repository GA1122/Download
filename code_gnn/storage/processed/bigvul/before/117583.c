SettingLevelBubbleDelegateView::SettingLevelBubbleDelegateView()
    : BubbleDelegateView(NULL, views::BubbleBorder::FLOAT),
      view_(NULL) {
  set_close_on_esc(false);
  set_use_focusless(true);
}
