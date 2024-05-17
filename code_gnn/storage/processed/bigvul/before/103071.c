void Browser::BookmarkCurrentPage() {
  UserMetrics::RecordAction(UserMetricsAction("Star"), profile_);

  BookmarkModel* model = profile()->GetBookmarkModel();
  if (!model || !model->IsLoaded())
    return;   

  GURL url;
  string16 title;
  TabContents* tab = GetSelectedTabContents();
  bookmark_utils::GetURLAndTitleToBookmark(tab, &url, &title);
  bool was_bookmarked = model->IsBookmarked(url);
  if (!was_bookmarked && profile_->IsOffTheRecord()) {
    tab->SaveFavicon();
  }
  model->SetURLStarred(url, title, true);
  if (window_->IsActive() && model->IsBookmarked(url)) {
    window_->ShowBookmarkBubble(url, was_bookmarked);
  }
}
