void WebContentsImpl::RequestOpenURL(RenderViewHost* rvh,
                                     const GURL& url,
                                     const Referrer& referrer,
                                     WindowOpenDisposition disposition,
                                     int64 source_frame_id,
                                     bool should_replace_current_entry,
                                     bool user_gesture) {
  if (static_cast<RenderViewHostImpl*>(rvh)->is_swapped_out() &&
      !rvh->GetSiteInstance()->IsRelatedSiteInstance(GetSiteInstance())) {
    return;
  }

  std::vector<GURL> redirect_chain;
  RequestTransferURL(url, redirect_chain, referrer, PAGE_TRANSITION_LINK,
                     disposition, source_frame_id, GlobalRequestID(),
                     should_replace_current_entry, user_gesture);
}
