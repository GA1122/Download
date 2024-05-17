void PageHandler::Navigate(const std::string& url,
                           Maybe<std::string> referrer,
                           Maybe<std::string> maybe_transition_type,
                           Maybe<std::string> frame_id,
                           std::unique_ptr<NavigateCallback> callback) {
  GURL gurl(url);
  if (!gurl.is_valid()) {
    callback->sendFailure(Response::Error("Cannot navigate to invalid URL"));
    return;
  }

  if (!host_) {
    callback->sendFailure(Response::InternalError());
    return;
  }

  ui::PageTransition type;
  std::string transition_type =
      maybe_transition_type.fromMaybe(Page::TransitionTypeEnum::Typed);
  if (transition_type == Page::TransitionTypeEnum::Link)
    type = ui::PAGE_TRANSITION_LINK;
  else if (transition_type == Page::TransitionTypeEnum::Typed)
    type = ui::PAGE_TRANSITION_TYPED;
  else if (transition_type == Page::TransitionTypeEnum::Address_bar)
    type = ui::PAGE_TRANSITION_FROM_ADDRESS_BAR;
  else if (transition_type == Page::TransitionTypeEnum::Auto_bookmark)
    type = ui::PAGE_TRANSITION_AUTO_BOOKMARK;
  else if (transition_type == Page::TransitionTypeEnum::Auto_subframe)
    type = ui::PAGE_TRANSITION_AUTO_SUBFRAME;
  else if (transition_type == Page::TransitionTypeEnum::Manual_subframe)
    type = ui::PAGE_TRANSITION_MANUAL_SUBFRAME;
  else if (transition_type == Page::TransitionTypeEnum::Generated)
    type = ui::PAGE_TRANSITION_GENERATED;
  else if (transition_type == Page::TransitionTypeEnum::Auto_toplevel)
    type = ui::PAGE_TRANSITION_AUTO_TOPLEVEL;
  else if (transition_type == Page::TransitionTypeEnum::Form_submit)
    type = ui::PAGE_TRANSITION_FORM_SUBMIT;
  else if (transition_type == Page::TransitionTypeEnum::Reload)
    type = ui::PAGE_TRANSITION_RELOAD;
  else if (transition_type == Page::TransitionTypeEnum::Keyword)
    type = ui::PAGE_TRANSITION_KEYWORD;
  else if (transition_type == Page::TransitionTypeEnum::Keyword_generated)
    type = ui::PAGE_TRANSITION_KEYWORD_GENERATED;
  else
    type = ui::PAGE_TRANSITION_TYPED;

  FrameTreeNode* frame_tree_node = nullptr;
  std::string out_frame_id = frame_id.fromMaybe(
      host_->frame_tree_node()->devtools_frame_token().ToString());
  FrameTreeNode* root = host_->frame_tree_node();
  if (root->devtools_frame_token().ToString() == out_frame_id) {
    frame_tree_node = root;
  } else {
    for (FrameTreeNode* node : root->frame_tree()->SubtreeNodes(root)) {
      if (node->devtools_frame_token().ToString() == out_frame_id) {
        frame_tree_node = node;
        break;
      }
    }
  }

  if (!frame_tree_node) {
    callback->sendFailure(Response::Error("No frame with given id found"));
    return;
  }

  NavigationController::LoadURLParams params(gurl);
  params.referrer =
      Referrer(GURL(referrer.fromMaybe("")), blink::kWebReferrerPolicyDefault);
  params.transition_type = type;
  params.frame_tree_node_id = frame_tree_node->frame_tree_node_id();
  frame_tree_node->navigator()->GetController()->LoadURLWithParams(params);

  base::UnguessableToken frame_token = frame_tree_node->devtools_frame_token();
  auto navigate_callback = navigate_callbacks_.find(frame_token);
  if (navigate_callback != navigate_callbacks_.end()) {
    std::string error_string = net::ErrorToString(net::ERR_ABORTED);
    navigate_callback->second->sendSuccess(out_frame_id, Maybe<std::string>(),
                                           Maybe<std::string>(error_string));
  }
  if (frame_tree_node->navigation_request()) {
    navigate_callbacks_[frame_token] = std::move(callback);
  } else {
    callback->sendSuccess(out_frame_id, Maybe<std::string>(),
                          Maybe<std::string>());
  }
}
