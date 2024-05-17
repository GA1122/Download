void Browser::ViewSourceForFrame(WebContents* source,
                                 const GURL& frame_url,
                                 const content::PageState& frame_page_state) {
  DCHECK(source);
  chrome::ViewSource(this, source, frame_url, frame_page_state);
}
