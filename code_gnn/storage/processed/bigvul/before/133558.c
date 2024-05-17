void WebContentsImpl::Find(int request_id,
                           const base::string16& search_text,
                           const blink::WebFindOptions& options) {
  Send(new ViewMsg_Find(GetRoutingID(), request_id, search_text, options));
}
