void Browser::BookmarkCurrentPage() {
  UserMetrics::RecordAction(UserMetricsAction("Star"));

  BookmarkModel* model = profile()->GetBookmarkModel();
  if (!model || !model->IsLoaded())
    return;   

  GURL url;
  string16 title;
  TabContentsWrapper* tab = GetSelectedTabContentsWrapper();
  bookmark_utils::GetURLAndTitleToBookmark(tab->tab_contents(), &url, &title);
  bool was_bookmarked = model->IsBookmarked(url);
  if (!was_bookmarked && profile_->IsOffTheRecord()) {
    tab->favicon_tab_helper()->SaveFavicon();
  }
  bookmark_utils::AddIfNotBookmarked(model, url, title);
  if (window_->IsActive() && model->IsBookmarked(url)) {
    window_->ShowBookmarkBubble(url, was_bookmarked);
  }
}
