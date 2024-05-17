RenderViewHostImpl::RenderViewHostImpl(
    SiteInstance* instance,
    RenderViewHostDelegate* delegate,
    RenderWidgetHostDelegate* widget_delegate,
    int routing_id,
    bool swapped_out,
    SessionStorageNamespace* session_storage)
    : RenderWidgetHostImpl(widget_delegate, instance->GetProcess(), routing_id),
      delegate_(delegate),
      instance_(static_cast<SiteInstanceImpl*>(instance)),
      waiting_for_drag_context_response_(false),
      enabled_bindings_(0),
      pending_request_id_(-1),
      navigations_suspended_(false),
      suspended_nav_message_(NULL),
      is_swapped_out_(swapped_out),
      is_subframe_(false),
      run_modal_reply_msg_(NULL),
      run_modal_opener_id_(MSG_ROUTING_NONE),
      is_waiting_for_beforeunload_ack_(false),
      is_waiting_for_unload_ack_(false),
      has_timed_out_on_unload_(false),
      unload_ack_is_for_cross_site_transition_(false),
      are_javascript_messages_suppressed_(false),
      sudden_termination_allowed_(false),
      session_storage_namespace_(
          static_cast<SessionStorageNamespaceImpl*>(session_storage)),
      save_accessibility_tree_for_testing_(false),
      render_view_termination_status_(base::TERMINATION_STATUS_STILL_RUNNING) {
  DCHECK(session_storage_namespace_);
  DCHECK(instance_);
  CHECK(delegate_);   

  GetProcess()->EnableSendQueue();

  GetContentClient()->browser()->RenderViewHostCreated(this);

  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_VIEW_HOST_CREATED,
      Source<RenderViewHost>(this),
      NotificationService::NoDetails());

#if defined(OS_ANDROID)
  media_player_manager_ = new MediaPlayerManagerAndroid(this);
#endif
}
