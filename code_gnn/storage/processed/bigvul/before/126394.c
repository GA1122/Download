void BrowserWindowGtk::OnBookmarkBarSizeAllocate(GtkWidget* sender,
                                                 GtkAllocation* allocation) {
  TRACE_EVENT0("ui::gtk", "BrowserWindowGtk::OnBookmarkBarSizeAllocate");
  if (infobar_container_->ContainsInfobars())
    InvalidateInfoBarBits();

  int arrow_size = InfoBar::kDefaultArrowTargetHeight;
  if (browser_->bookmark_bar_state() != BookmarkBar::DETACHED)
    arrow_size += allocation->height;
  infobar_container_->SetMaxTopArrowHeight(arrow_size);
}
