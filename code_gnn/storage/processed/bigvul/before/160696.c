RenderFrameImpl::PendingNavigationInfo::PendingNavigationInfo(
    const NavigationPolicyInfo& info)
    : navigation_type(info.navigation_type),
      policy(info.default_policy),
      replaces_current_history_item(info.replaces_current_history_item),
      history_navigation_in_new_child_frame(
          info.is_history_navigation_in_new_child_frame),
      client_redirect(info.is_client_redirect),
      triggering_event_info(info.triggering_event_info),
      form(info.form),
      source_location(info.source_location) {}
