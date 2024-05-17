bool BrowserView::MaybeShowBookmarkBar(WebContents* contents) {
  const bool show_bookmark_bar =
      contents && browser_->SupportsWindowFeature(Browser::FEATURE_BOOKMARKBAR);
  if (!show_bookmark_bar && !bookmark_bar_view_.get())
    return false;
  if (!bookmark_bar_view_.get()) {
    bookmark_bar_view_.reset(new BookmarkBarView(browser_.get(), this));
    bookmark_bar_view_->set_owned_by_client();
    bookmark_bar_view_->SetBackground(
        std::make_unique<BookmarkBarViewBackground>(this,
                                                    bookmark_bar_view_.get()));
    bookmark_bar_view_->SetBookmarkBarState(
        browser_->bookmark_bar_state(),
        BookmarkBar::DONT_ANIMATE_STATE_CHANGE);
    GetBrowserViewLayout()->set_bookmark_bar(bookmark_bar_view_.get());
  }
  bookmark_bar_view_->SetPageNavigator(GetActiveWebContents());

  bool needs_layout = false;
  views::View* new_parent = nullptr;
  if (show_bookmark_bar) {
    if (bookmark_bar_view_->IsDetached())
      new_parent = this;
    else
      new_parent = top_container_;
  }
  if (new_parent != bookmark_bar_view_->parent()) {
    SetBookmarkBarParent(new_parent);
    needs_layout = true;
  }

  if (bookmark_bar_view_->GetPreferredSize().height() !=
      bookmark_bar_view_->height())
    needs_layout = true;

  return needs_layout;
}
