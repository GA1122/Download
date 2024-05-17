void WebContentsImpl::RequestTransferURL(
    const GURL& url,
    const Referrer& referrer,
    WindowOpenDisposition disposition,
    int64 source_frame_id,
    const GlobalRequestID& old_request_id,
    bool should_replace_current_entry,
    bool user_gesture) {
  WebContents* new_contents = NULL;
  PageTransition transition_type = PAGE_TRANSITION_LINK;
  if (render_manager_.web_ui()) {
    OpenURLParams params(url, Referrer(), source_frame_id, disposition,
        render_manager_.web_ui()->GetLinkTransitionType(),
        false  );
    params.transferred_global_request_id = old_request_id;
    new_contents = OpenURL(params);
    transition_type = render_manager_.web_ui()->GetLinkTransitionType();
  } else {
    OpenURLParams params(url, referrer, source_frame_id, disposition,
        PAGE_TRANSITION_LINK, true  );
    params.transferred_global_request_id = old_request_id;
    params.should_replace_current_entry = should_replace_current_entry;
    params.user_gesture = user_gesture;
    new_contents = OpenURL(params);
  }
  if (new_contents) {
    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      DidOpenRequestedURL(new_contents,
                                          url,
                                          referrer,
                                          disposition,
                                          transition_type,
                                          source_frame_id));
  }
}
