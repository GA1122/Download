void BrowserView::ShowOneClickSigninBubble(
    const StartSyncCallback& start_sync_callback) {
  OneClickSigninBubbleView::ShowBubble(toolbar_->app_menu(),
                                       start_sync_callback);
}
