void WebContentsImpl::CopyMaxPageIDsFrom(WebContents* web_contents) {
  WebContentsImpl* contents = static_cast<WebContentsImpl*>(web_contents);
  max_page_ids_ = contents->max_page_ids_;
}
