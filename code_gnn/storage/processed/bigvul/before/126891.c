void BrowserView::WebContentsFocused(WebContents* contents) {
  if (contents_container_->GetWebContents() == contents)
    contents_container_->OnWebContentsFocused(contents);
  else
    devtools_container_->OnWebContentsFocused(contents);
}
