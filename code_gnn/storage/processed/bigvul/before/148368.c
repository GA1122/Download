void WebContentsImpl::Find(int request_id,
                           const base::string16& search_text,
                           const blink::WebFindOptions& options) {
  if (search_text.empty()) {
    NOTREACHED();
    return;
  }

  GetOrCreateFindRequestManager()->Find(request_id, search_text, options);
}
