void Browser::ViewSourceForTab(WebContents* source, const GURL& page_url) {
  DCHECK(source);
  TabContents* tab_contents = chrome::GetTabContentsAt(this,
      tab_strip_model_->GetIndexOfWebContents(source));
  chrome::ViewSource(this, tab_contents);
}
