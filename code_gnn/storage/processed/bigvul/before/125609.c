void RenderViewHostImpl::Find(int request_id,
                              const string16& search_text,
                              const WebKit::WebFindOptions& options) {
  Send(new ViewMsg_Find(GetRoutingID(), request_id, search_text, options));
}
