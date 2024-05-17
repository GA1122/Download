void RenderViewHostImpl::SynchronousFind(int request_id,
                                         const string16& search_text,
                                         const WebKit::WebFindOptions& options,
                                         int* match_count,
                                         int* active_ordinal) {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableWebViewSynchronousAPIs)) {
    return;
  }

  Send(new ViewMsg_SynchronousFind(GetRoutingID(), request_id, search_text,
                                   options, match_count, active_ordinal));
}
