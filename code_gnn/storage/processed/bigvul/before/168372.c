void BrowserView::TabDetachedAt(WebContents* contents,
                                int index,
                                bool was_active) {
  if (was_active) {
    web_contents_close_handler_->ActiveTabChanged();
    contents_web_view_->SetWebContents(nullptr);
    infobar_container_->ChangeInfoBarManager(nullptr);
    UpdateDevToolsForContents(nullptr, true);
  }
}
