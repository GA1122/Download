void RenderViewHostImpl::OnContextMenu(const ContextMenuParams& params) {
  ContextMenuParams validated_params(params);
  RenderProcessHost* process = GetProcess();
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  FilterURL(policy, process, true, &validated_params.link_url);
  FilterURL(policy, process, true, &validated_params.src_url);
  FilterURL(policy, process, false, &validated_params.page_url);
  FilterURL(policy, process, true, &validated_params.frame_url);

  ContextMenuSourceType type = CONTEXT_MENU_SOURCE_MOUSE;
  if (!in_process_event_types_.empty()) {
    WebKit::WebInputEvent::Type event_type = in_process_event_types_.front();
    if (WebKit::WebInputEvent::isGestureEventType(event_type))
      type = CONTEXT_MENU_SOURCE_TOUCH;
    else if (WebKit::WebInputEvent::isKeyboardEventType(event_type))
      type = CONTEXT_MENU_SOURCE_KEYBOARD;
  }
  delegate_->ShowContextMenu(validated_params, type);
}
