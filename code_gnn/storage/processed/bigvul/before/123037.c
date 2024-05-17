RenderWidgetHostImpl::RenderWidgetHostImpl(RenderWidgetHostDelegate* delegate,
                                           RenderProcessHost* process,
                                           int routing_id)
    : view_(NULL),
      renderer_initialized_(false),
      hung_renderer_delay_ms_(kHungRendererDelayMs),
      delegate_(delegate),
      process_(process),
      routing_id_(routing_id),
      surface_id_(0),
      is_loading_(false),
      is_hidden_(false),
      is_fullscreen_(false),
      is_accelerated_compositing_active_(false),
      repaint_ack_pending_(false),
      resize_ack_pending_(false),
      should_auto_resize_(false),
      waiting_for_screen_rects_ack_(false),
      mouse_move_pending_(false),
      mouse_wheel_pending_(false),
      select_range_pending_(false),
      needs_repainting_on_restore_(false),
      is_unresponsive_(false),
      in_flight_event_count_(0),
      in_get_backing_store_(false),
      abort_get_backing_store_(false),
      view_being_painted_(false),
      ignore_input_events_(false),
      text_direction_updated_(false),
      text_direction_(WebKit::WebTextDirectionLeftToRight),
      text_direction_canceled_(false),
      suppress_next_char_events_(false),
      pending_mouse_lock_request_(false),
      allow_privileged_mouse_lock_(false),
      has_touch_handler_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_factory_(this)),
      tick_active_smooth_scroll_gestures_task_posted_(false),
      touch_event_queue_(new TouchEventQueue(this)),
      gesture_event_filter_(new GestureEventFilter(this)) {
  CHECK(delegate_);
  if (routing_id_ == MSG_ROUTING_NONE) {
    routing_id_ = process_->GetNextRoutingID();
    surface_id_ = GpuSurfaceTracker::Get()->AddSurfaceForRenderer(
        process_->GetID(),
        routing_id_);
  } else {
    surface_id_ = GpuSurfaceTracker::Get()->LookupSurfaceForRenderer(
        process_->GetID(),
        routing_id_);
    DCHECK(surface_id_);
  }

  is_threaded_compositing_enabled_ = IsThreadedCompositingEnabled();

  process_->Attach(this, routing_id_);
  process_->WidgetRestored();

#if defined(USE_AURA)
  bool overscroll_enabled = CommandLine::ForCurrentProcess()->
      HasSwitch(switches::kEnableOverscrollHistoryNavigation);
  if (overscroll_enabled)
    InitializeOverscrollController();
#endif
}
