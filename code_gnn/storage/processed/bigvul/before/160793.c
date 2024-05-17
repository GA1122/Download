void RenderViewImpl::Initialize(
    mojom::CreateViewParamsPtr params,
    const RenderWidget::ShowCallback& show_callback) {
  bool was_created_by_renderer = !show_callback.is_null();
#if defined(OS_ANDROID)
  was_created_by_renderer_ = was_created_by_renderer;
#endif
  renderer_wide_named_frame_lookup_ = params->renderer_wide_named_frame_lookup;
  display_mode_ = params->initial_size.display_mode;

  WebFrame* opener_frame =
      RenderFrameImpl::ResolveOpener(params->opener_frame_route_id);

  webview_ =
      WebView::Create(this,
                      is_hidden() ? blink::mojom::PageVisibilityState::kHidden
                                  : blink::mojom::PageVisibilityState::kVisible,
                      opener_frame ? opener_frame->View() : nullptr);
  RenderWidget::Init(show_callback, webview_->GetWidget());

  g_view_map.Get().insert(std::make_pair(webview(), this));
  g_routing_id_view_map.Get().insert(std::make_pair(GetRoutingID(), this));

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();

  if (command_line.HasSwitch(switches::kStatsCollectionController))
    stats_collection_observer_.reset(new StatsCollectionObserver(this));

  webview()->SetDisplayMode(display_mode_);
  webview()->GetSettings()->SetPreferCompositingToLCDTextEnabled(
      PreferCompositingToLCDText(compositor_deps_, device_scale_factor_));
  webview()->GetSettings()->SetThreadedScrollingEnabled(
      !command_line.HasSwitch(switches::kDisableThreadedScrolling));
  webview()->SetShowFPSCounter(
      command_line.HasSwitch(cc::switches::kShowFPSCounter));

  ApplyWebPreferencesInternal(webkit_preferences_, webview(), compositor_deps_);

  if (switches::IsTouchDragDropEnabled())
    webview()->GetSettings()->SetTouchDragDropEnabled(true);

  webview()->GetSettings()->SetBrowserSideNavigationEnabled(
      IsBrowserSideNavigationEnabled());

  WebSettings::SelectionStrategyType selection_strategy =
      WebSettings::SelectionStrategyType::kCharacter;
  const std::string selection_strategy_str =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kTouchTextSelectionStrategy);
  if (selection_strategy_str == "direction")
    selection_strategy = WebSettings::SelectionStrategyType::kDirection;
  webview()->GetSettings()->SetSelectionStrategy(selection_strategy);

  std::string passiveListenersDefault =
      command_line.GetSwitchValueASCII(switches::kPassiveListenersDefault);
  if (!passiveListenersDefault.empty()) {
    WebSettings::PassiveEventListenerDefault passiveDefault =
        WebSettings::PassiveEventListenerDefault::kFalse;
    if (passiveListenersDefault == "true")
      passiveDefault = WebSettings::PassiveEventListenerDefault::kTrue;
    else if (passiveListenersDefault == "forcealltrue")
      passiveDefault = WebSettings::PassiveEventListenerDefault::kForceAllTrue;
    webview()->GetSettings()->SetPassiveEventListenerDefault(passiveDefault);
  }

  ApplyBlinkSettings(command_line, webview()->GetSettings());

  if (params->main_frame_routing_id != MSG_ROUTING_NONE) {
    CHECK(params->main_frame_interface_provider.is_valid());
    service_manager::mojom::InterfaceProviderPtr main_frame_interface_provider(
        std::move(params->main_frame_interface_provider));
    main_render_frame_ = RenderFrameImpl::CreateMainFrame(
        this, params->main_frame_routing_id,
        std::move(main_frame_interface_provider),
        params->main_frame_widget_routing_id, params->hidden, screen_info(),
        compositor_deps_, opener_frame, params->devtools_main_frame_token,
        params->replicated_frame_state, params->has_committed_real_load);
  }

  if (params->proxy_routing_id != MSG_ROUTING_NONE) {
    CHECK(params->swapped_out);
    RenderFrameProxy::CreateFrameProxy(params->proxy_routing_id, GetRoutingID(),
                                       opener_frame, MSG_ROUTING_NONE,
                                       params->replicated_frame_state);
  }

  if (main_render_frame_)
    main_render_frame_->Initialize();

  if (!was_created_by_renderer)
    did_show_ = true;

  if (params->window_was_created_with_opener)
    webview()->SetOpenedByDOM();

  UpdateWebViewWithDeviceScaleFactor();
  OnSetRendererPrefs(params->renderer_preferences);

  if (!params->enable_auto_resize) {
    OnResize(params->initial_size);
  } else {
    OnEnableAutoResize(params->min_size, params->max_size);
  }

  idle_user_detector_.reset(new IdleUserDetector(this));

  GetContentClient()->renderer()->RenderViewCreated(this);

  page_zoom_level_ = params->page_zoom_level;
}
