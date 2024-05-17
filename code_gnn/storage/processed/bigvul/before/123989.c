void RenderViewImpl::show(WebNavigationPolicy policy) {
  if (did_show_) {
#if defined(OS_ANDROID)
    if (!webkit_preferences_.supports_multiple_windows)
      return;
#endif
    NOTREACHED() << "received extraneous Show call";
    return;
  }
  did_show_ = true;

  DCHECK(opener_id_ != MSG_ROUTING_NONE);

  if (GetContentClient()->renderer()->AllowPopup())
    opened_by_user_gesture_ = true;

  if (!opened_by_user_gesture_) {
    if (policy != WebKit::WebNavigationPolicyNewBackgroundTab)
      policy = WebKit::WebNavigationPolicyNewPopup;
  }

  Send(new ViewHostMsg_ShowView(opener_id_, routing_id_,
      NavigationPolicyToDisposition(policy), initial_pos_,
      opened_by_user_gesture_));
  SetPendingWindowRect(initial_pos_);
}
