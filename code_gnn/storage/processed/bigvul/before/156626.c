void NavigatorImpl::OnBeforeUnloadACK(FrameTreeNode* frame_tree_node,
                                      bool proceed,
                                      const base::TimeTicks& proceed_time) {
  DCHECK(frame_tree_node);

  NavigationRequest* navigation_request = frame_tree_node->navigation_request();

  if (!navigation_request)
    return;

  if (!proceed) {
    CancelNavigation(frame_tree_node, true);
    return;
  }

  if (navigation_request->state() >
      NavigationRequest::WAITING_FOR_RENDERER_RESPONSE) {
    DCHECK(navigation_request->from_begin_navigation());
    return;
  }

  navigation_request->set_navigation_start_time(proceed_time);

  DCHECK_EQ(NavigationRequest::WAITING_FOR_RENDERER_RESPONSE,
            navigation_request->state());

  navigation_request->BeginNavigation();
}
