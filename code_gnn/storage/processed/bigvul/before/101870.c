void Browser::UpdateTargetURL(TabContents* source, int32 page_id,
                              const GURL& url) {
  Browser::UpdateTargetURLHelper(source, page_id, url);

  if (!GetStatusBubble())
    return;

  if (source == GetSelectedTabContents()) {
    PrefService* prefs = profile_->GetPrefs();
    GetStatusBubble()->SetURL(url, prefs->GetString(prefs::kAcceptLanguages));
  }
}
