void Browser::URLStarredChanged(content::WebContents* web_contents,
                                bool starred) {
  if (web_contents == chrome::GetActiveWebContents(this))
    window_->SetStarredState(starred);
}
