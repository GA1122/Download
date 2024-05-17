void TabContentsContainerGtk::WebContentsDestroyed(WebContents* contents) {
  if (preview_ && contents == preview_->web_contents())
    SetPreview(NULL);
  else if (tab_ && contents == tab_->web_contents())
    SetTab(NULL);
  else
    NOTREACHED();
}
