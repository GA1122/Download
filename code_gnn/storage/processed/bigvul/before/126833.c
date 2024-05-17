void BrowserView::OnWidgetActivationChanged(views::Widget* widget,
                                            bool active) {
#if defined(USE_ASH)
  if (launcher_item_controller_.get())
    launcher_item_controller_->BrowserActivationStateChanged();
#endif

  if (active) {
    BrowserList::SetLastActive(browser_.get());
    browser_->OnWindowActivated();
  }
}
