RenderWidgetHostImpl::RenderWidgetHostImpl(RenderWidgetHostDelegate* delegate,
                                           RenderProcessHost* process,
                                           int32_t routing_id,
                                           mojom::WidgetPtr widget,
                                           bool hidden)
    : renderer_initialized_(false),
      destroyed_(false),
      delegate_(delegate),
      owner_delegate_(nullptr),
      process_(process),
      routing_id_(routing_id),
      clock_(base::DefaultTickClock::GetInstance()),
      is_loading_(false),
      is_hidden_(hidden),
      visual_properties_ack_pending_(false),
      auto_resize_enabled_(false),
      waiting_for_screen_rects_ack_(false),
      needs_repainting_on_restore_(false),
      is_unresponsive_(false),
      in_flight_event_count_(0),
      in_get_backing_store_(false),
      ignore_input_events_(false),
      text_direction_updated_(false),
      text_direction_(blink::kWebTextDirectionLeftToRight),
      text_direction_canceled_(false),
      suppress_events_until_keydown_(false),
      pending_mouse_lock_request_(false),
      allow_privileged_mouse_lock_(false),
      is_last_unlocked_by_target_(false),
      has_touch_handler_(false),
      is_in_touchpad_gesture_fling_(false),
      latency_tracker_(delegate_),
      next_browser_snapshot_id_(1),
      owned_by_render_frame_host_(false),
      is_focused_(false),
      hung_renderer_delay_(TimeDelta::FromMilliseconds(kHungRendererDelayMs)),
      new_content_rendering_delay_(
          TimeDelta::FromMilliseconds(kNewContentRenderingDelayMs)),
      current_content_source_id_(0),
      monitoring_composition_info_(false),
      compositor_frame_sink_binding_(this),
      frame_token_message_queue_(
          std::make_unique<FrameTokenMessageQueue>(this)),
      render_frame_metadata_provider_(
#if defined(OS_MACOSX)
          ui::WindowResizeHelperMac::Get()->task_runner(),
#else
          base::ThreadTaskRunnerHandle::Get(),
#endif
          frame_token_message_queue_.get()),
      frame_sink_id_(base::checked_cast<uint32_t>(process_->GetID()),
                     base::checked_cast<uint32_t>(routing_id_)),
      weak_factory_(this) {
#if defined(OS_MACOSX)
  fling_scheduler_ = std::make_unique<FlingSchedulerMac>(this);
#elif defined(OS_ANDROID)
  fling_scheduler_ = std::make_unique<FlingSchedulerAndroid>(this);
#else
  fling_scheduler_ = std::make_unique<FlingScheduler>(this);
#endif
  CHECK(delegate_);
  CHECK_NE(MSG_ROUTING_NONE, routing_id_);
  DCHECK(base::TaskScheduler::GetInstance())
      << "Ref. Prerequisite section of post_task.h";

  std::pair<RoutingIDWidgetMap::iterator, bool> result =
      g_routing_id_widget_map.Get().insert(std::make_pair(
          RenderWidgetHostID(process->GetID(), routing_id_), this));
  CHECK(result.second) << "Inserting a duplicate item!";
  process_->AddRoute(routing_id_, this);
  process_->AddWidget(this);

  SetupInputRouter();
  SetWidget(std::move(widget));

  const auto* command_line = base::CommandLine::ForCurrentProcess();
  if (!command_line->HasSwitch(switches::kDisableHangMonitor)) {
    input_event_ack_timeout_.reset(new TimeoutMonitor(
        base::BindRepeating(&RenderWidgetHostImpl::OnInputEventAckTimeout,
                            weak_factory_.GetWeakPtr())));
  }

  if (!command_line->HasSwitch(switches::kDisableNewContentRenderingTimeout)) {
    new_content_rendering_timeout_.reset(new TimeoutMonitor(
        base::Bind(&RenderWidgetHostImpl::ClearDisplayedGraphics,
                   weak_factory_.GetWeakPtr())));
  }

  enable_surface_synchronization_ = features::IsSurfaceSynchronizationEnabled();
  enable_viz_ = base::FeatureList::IsEnabled(features::kVizDisplayCompositor);

  if (!enable_viz_) {
#if !defined(OS_ANDROID)
    if (BrowserMainLoop* main_loop = BrowserMainLoop::GetInstance())
      shared_bitmap_manager_ = main_loop->GetServerSharedBitmapManager();
#endif
  }

  delegate_->RenderWidgetCreated(this);
  render_frame_metadata_provider_.AddObserver(this);
}
