void RenderViewImpl::Initialize(
    mojom::CreateViewParamsPtr params,
    RenderWidget::ShowCallback show_callback,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner) {
  bool has_show_callback = !!show_callback;
#if defined(OS_ANDROID)
  was_created_by_renderer_ = has_show_callback;
#endif

  WebFrame* opener_frame =
      RenderFrameImpl::ResolveOpener(params->opener_frame_route_id);

  webview_ =
      WebView::Create(this, WidgetClient(),
                      is_hidden() ? blink::mojom::PageVisibilityState::kHidden
                                  : blink::mojom::PageVisibilityState::kVisible,
                      opener_frame ? opener_frame->View() : nullptr);
  RenderWidget::Init(std::move(show_callback), webview_->GetWidget());

  g_view_map.Get().insert(std::make_pair(webview(), this));
  g_routing_id_view_map.Get().insert(std::make_pair(GetRoutingID(), this));

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();

  if (command_line.HasSwitch(switches::kStatsCollectionController))
    stats_collection_observer_.reset(new StatsCollectionObserver(this));

  webview()->SetDisplayMode(GetWidget()->display_mode());
  webview()->GetSettings()->SetThreadedScrollingEnabled(
      !command_line.HasSwitch(switches::kDisableThreadedScrolling));
  webview()->SetShowFPSCounter(
      command_line.HasSwitch(cc::switches::kShowFPSCounter));

  ApplyWebPreferencesInternal(webkit_preferences_, webview());

  if (switches::IsTouchDragDropEnabled())
    webview()->GetSettings()->SetTouchDragDropEnabled(true);

  WebSettings::SelectionStrategyType selection_strategy =
      WebSettings::SelectionStrategyType::kCharacter;
  const std::string selection_strategy_str =
      command_line.GetSwitchValueASCII(switches::kTouchTextSelectionStrategy);
  if (selection_strategy_str == "direction")
    selection_strategy = WebSettings::SelectionStrategyType::kDirection;
  webview()->GetSettings()->SetSelectionStrategy(selection_strategy);

  std::string passive_listeners_default =
      command_line.GetSwitchValueASCII(switches::kPassiveListenersDefault);
  if (!passive_listeners_default.empty()) {
    WebSettings::PassiveEventListenerDefault passiveDefault =
        WebSettings::PassiveEventListenerDefault::kFalse;
    if (passive_listeners_default == "true")
      passiveDefault = WebSettings::PassiveEventListenerDefault::kTrue;
    else if (passive_listeners_default == "forcealltrue")
      passiveDefault = WebSettings::PassiveEventListenerDefault::kForceAllTrue;
    webview()->GetSettings()->SetPassiveEventListenerDefault(passiveDefault);
  }

  std::string network_quiet_timeout =
      command_line.GetSwitchValueASCII(switches::kNetworkQuietTimeout);
  if (!network_quiet_timeout.empty()) {
    double network_quiet_timeout_seconds = 0.0;
    if (base::StringToDouble(network_quiet_timeout,
                             &network_quiet_timeout_seconds)) {
      webview()->GetSettings()->SetNetworkQuietTimeout(
          network_quiet_timeout_seconds);
    }
  }

  ApplyBlinkSettings(command_line, webview()->GetSettings());

  if (params->main_frame_routing_id != MSG_ROUTING_NONE) {
    CHECK(params->main_frame_interface_provider.is_valid());
    service_manager::mojom::InterfaceProviderPtr main_frame_interface_provider(
        std::move(params->main_frame_interface_provider));
    main_render_frame_ = RenderFrameImpl::CreateMainFrame(
        this, params->main_frame_routing_id,
        std::move(main_frame_interface_provider),
        params->main_frame_widget_routing_id, params->hidden,
        GetWidget()->GetWebScreenInfo(), GetWidget()->compositor_deps(),
        opener_frame, params->devtools_main_frame_token,
        params->replicated_frame_state, params->has_committed_real_load);
  }

  if (params->proxy_routing_id != MSG_ROUTING_NONE) {
    CHECK(params->swapped_out);
    RenderFrameProxy::CreateFrameProxy(params->proxy_routing_id, GetRoutingID(),
                                       opener_frame, MSG_ROUTING_NONE,
                                       params->replicated_frame_state,
                                       params->devtools_main_frame_token);
  }

  if (main_render_frame_)
    main_render_frame_->Initialize();

  if (!has_show_callback)
    RenderWidget::set_did_show();

  if (params->window_was_created_with_opener)
    webview()->SetOpenedByDOM();

  UpdateWebViewWithDeviceScaleFactor();
  OnSetRendererPrefs(params->renderer_preferences);
  OnSynchronizeVisualProperties(params->visual_properties);

  GetContentClient()->renderer()->RenderViewCreated(this);
  page_zoom_level_ = 0;

  nav_state_sync_timer_.SetTaskRunner(task_runner);
}
