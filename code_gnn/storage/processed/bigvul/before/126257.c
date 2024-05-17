void Browser::UpdateTargetURL(WebContents* source, int32 page_id,
                              const GURL& url) {
  if (!GetStatusBubble())
    return;

  if (source == chrome::GetActiveWebContents(this)) {
    PrefService* prefs = profile_->GetPrefs();
    GetStatusBubble()->SetURL(url, prefs->GetString(prefs::kAcceptLanguages));
  }
}
