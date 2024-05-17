SettingLevelBubbleView* SettingLevelBubble::CreateView() {
  SettingLevelBubbleDelegateView* delegate = new SettingLevelBubbleDelegateView;
  views::Widget* widget = browser::CreateViewsBubbleAboveLockScreen(delegate);
  widget->AddObserver(this);
  return delegate->view();
}
