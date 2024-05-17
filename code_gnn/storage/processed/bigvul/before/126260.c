void Browser::ViewSourceForFrame(WebContents* source,
                                 const GURL& frame_url,
                                 const std::string& frame_content_state) {
  DCHECK(source);
  TabContents* tab_contents = chrome::GetTabContentsAt(this,
      tab_strip_model_->GetIndexOfWebContents(source));
  chrome::ViewSource(this, tab_contents, frame_url, frame_content_state);
}
