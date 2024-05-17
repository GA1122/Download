bool BrowserView::MaybeShowBookmarkBar(TabContents* contents) {
  views::View* new_bookmark_bar_view = NULL;
  if (browser_->SupportsWindowFeature(Browser::FEATURE_BOOKMARKBAR) &&
      contents) {
    if (!bookmark_bar_view_.get()) {
      bookmark_bar_view_.reset(new BookmarkBarView(browser_.get(), this));
      bookmark_bar_view_->set_owned_by_client();
      bookmark_bar_view_->set_background(
          new BookmarkExtensionBackground(this, bookmark_bar_view_.get(),
                                          browser_.get()));
      bookmark_bar_view_->SetBookmarkBarState(
          browser_->bookmark_bar_state(),
          BookmarkBar::DONT_ANIMATE_STATE_CHANGE,
          browser_->search_model()->mode());
    }
    bookmark_bar_view_->SetPageNavigator(contents->web_contents());
    new_bookmark_bar_view = bookmark_bar_view_.get();
  }
  return UpdateChildViewAndLayout(new_bookmark_bar_view, &active_bookmark_bar_);
}
