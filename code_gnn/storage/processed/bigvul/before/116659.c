RenderViewImpl::RenderViewImpl(
    gfx::NativeViewId parent_hwnd,
    int32 opener_id,
    const content::RendererPreferences& renderer_prefs,
    const WebPreferences& webkit_prefs,
    SharedRenderViewCounter* counter,
    int32 routing_id,
    int32 surface_id,
    int64 session_storage_namespace_id,
    const string16& frame_name,
    int32 next_page_id,
    const WebKit::WebScreenInfo& screen_info)
    : RenderWidget(WebKit::WebPopupTypeNone, screen_info),
      webkit_preferences_(webkit_prefs),
      send_content_state_immediately_(false),
      enabled_bindings_(0),
      send_preferred_size_changes_(false),
      is_loading_(false),
      navigation_gesture_(NavigationGestureUnknown),
      opened_by_user_gesture_(true),
      opener_suppressed_(false),
      page_id_(-1),
      last_page_id_sent_to_browser_(-1),
      next_page_id_(next_page_id),
      history_list_offset_(-1),
      history_list_length_(0),
      target_url_status_(TARGET_NONE),
      selection_text_offset_(0),
      cached_is_main_frame_pinned_to_left_(false),
      cached_is_main_frame_pinned_to_right_(false),
      cached_has_main_frame_horizontal_scrollbar_(false),
      cached_has_main_frame_vertical_scrollbar_(false),
      context_has_swapbuffers_complete_callback_(false),
      queried_for_swapbuffers_complete_callback_(false),
      context_is_web_graphics_context_3d_command_buffer_impl_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(cookie_jar_(this)),
      geolocation_dispatcher_(NULL),
      input_tag_speech_dispatcher_(NULL),
      device_orientation_dispatcher_(NULL),
      media_stream_dispatcher_(NULL),
      p2p_socket_dispatcher_(NULL),
      devtools_agent_(NULL),
      renderer_accessibility_(NULL),
      mouse_lock_dispatcher_(NULL),
      session_storage_namespace_id_(session_storage_namespace_id),
      handling_select_range_(false),
#if defined(OS_WIN)
      focused_plugin_id_(-1),
#endif
      ALLOW_THIS_IN_INITIALIZER_LIST(pepper_delegate_(this)) {
  routing_id_ = routing_id;
  surface_id_ = surface_id;
  if (opener_id != MSG_ROUTING_NONE)
    opener_id_ = opener_id;

  DCHECK_GE(next_page_id_, 0);

#if defined(ENABLE_NOTIFICATIONS)
  notification_provider_ = new NotificationProvider(this);
#else
  notification_provider_ = NULL;
#endif

  webwidget_ = WebView::create(this);
  webwidget_mouse_lock_target_.reset(new WebWidgetLockTarget(webwidget_));

  if (counter) {
    shared_popup_counter_ = counter;
    shared_popup_counter_->data++;
    decrement_shared_popup_at_destruction_ = true;
  } else {
    shared_popup_counter_ = new SharedRenderViewCounter(0);
    decrement_shared_popup_at_destruction_ = false;
  }

  RenderThread::Get()->AddRoute(routing_id_, this);
  AddRef();

  if (opener_id == MSG_ROUTING_NONE) {
    did_show_ = true;
    CompleteInit(parent_hwnd);
  }

  g_view_map.Get().insert(std::make_pair(webview(), this));
  webkit_preferences_.Apply(webview());
  webview()->initializeMainFrame(this);
  if (!frame_name.empty())
    webview()->mainFrame()->setName(frame_name);
  webview()->settings()->setMinimumTimerInterval(
      is_hidden() ? webkit_glue::kBackgroundTabTimerInterval :
          webkit_glue::kForegroundTabTimerInterval);

  OnSetRendererPrefs(renderer_prefs);

  host_window_ = parent_hwnd;

#if defined(ENABLE_P2P_APIS)
  if (!p2p_socket_dispatcher_)
    p2p_socket_dispatcher_ = new content::P2PSocketDispatcher(this);
#endif

#if defined(ENABLE_WEBRTC)
  if (!media_stream_dispatcher_)
    media_stream_dispatcher_ = new MediaStreamDispatcher(this);
#endif

  new MHTMLGenerator(this);
#if defined(OS_MACOSX)
  new TextInputClientObserver(this);
#endif   

  devtools_agent_ = new DevToolsAgent(this);
  renderer_accessibility_ = new RendererAccessibility(this);
  mouse_lock_dispatcher_ = new MouseLockDispatcher(this);
  intents_host_ = new WebIntentsHost(this);

  new IdleUserDetector(this);

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDomAutomationController))
    enabled_bindings_ |= content::BINDINGS_POLICY_DOM_AUTOMATION;

  bool enable_fixed_layout =
      command_line.HasSwitch(switches::kEnableFixedLayout);
  webview()->enableFixedLayoutMode(enable_fixed_layout);
  if (enable_fixed_layout)
      webview()->settings()->setFixedElementsLayoutRelativeToFrame(true);

  content::GetContentClient()->renderer()->RenderViewCreated(this);
}
