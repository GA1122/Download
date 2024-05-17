void RenderFrameHostImpl::NavigateToURL(const GURL& url) {
  FrameMsg_Navigate_Params params;
  params.page_id = -1;
  params.pending_history_list_offset = -1;
  params.current_history_list_offset = -1;
  params.current_history_list_length = 0;
  params.url = url;
  params.transition = PAGE_TRANSITION_LINK;
  params.navigation_type = FrameMsg_Navigate_Type::NORMAL;
  Navigate(params);
}
