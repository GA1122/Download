void NavigatorImpl::OnBeforeUnloadACK(FrameTreeNode* frame_tree_node,
                                      bool proceed) {
  CHECK(IsBrowserSideNavigationEnabled());
  DCHECK(frame_tree_node);

  NavigationRequest* navigation_request = frame_tree_node->navigation_request();

  if (!navigation_request)
    return;

  DCHECK_EQ(NavigationRequest::WAITING_FOR_RENDERER_RESPONSE,
            navigation_request->state());

  if (proceed)
    navigation_request->BeginNavigation();
  else
    CancelNavigation(frame_tree_node);
}
