void WebContentsImpl::Init(const WebContents::CreateParams& params) {
  visibility_ =
      params.initially_hidden ? Visibility::HIDDEN : Visibility::VISIBLE;

  if (!params.last_active_time.is_null())
    last_active_time_ = params.last_active_time;

  DCHECK((params.routing_id == MSG_ROUTING_NONE &&
          params.main_frame_routing_id == MSG_ROUTING_NONE &&
          params.main_frame_widget_routing_id == MSG_ROUTING_NONE) ||
         (params.routing_id != MSG_ROUTING_NONE &&
          params.main_frame_routing_id != MSG_ROUTING_NONE &&
          params.main_frame_widget_routing_id != MSG_ROUTING_NONE));

  scoped_refptr<SiteInstance> site_instance = params.site_instance;
  if (!site_instance)
    site_instance = SiteInstance::Create(params.browser_context);
  if (params.desired_renderer_state == CreateParams::kNoRendererProcess) {
    static_cast<SiteInstanceImpl*>(site_instance.get())
        ->PreventAssociationWithSpareProcess();
  }

  int32_t view_routing_id = params.routing_id;
  int32_t main_frame_widget_routing_id = params.main_frame_widget_routing_id;
  if (main_frame_widget_routing_id == MSG_ROUTING_NONE) {
    view_routing_id = site_instance->GetProcess()->GetNextRoutingID();
    main_frame_widget_routing_id =
        site_instance->GetProcess()->GetNextRoutingID();
  }

  DCHECK_NE(view_routing_id, main_frame_widget_routing_id);

  GetRenderManager()->Init(
      site_instance.get(), view_routing_id, params.main_frame_routing_id,
      main_frame_widget_routing_id, params.renderer_initiated_creation);

  std::string unique_name;
  frame_tree_.root()->SetFrameName(params.main_frame_name, unique_name);

  WebContentsViewDelegate* delegate =
      GetContentClient()->browser()->GetWebContentsViewDelegate(this);

  if (GuestMode::IsCrossProcessFrameGuest(this)) {
    view_.reset(new WebContentsViewChildFrame(
        this, delegate, &render_view_host_delegate_view_));
  } else {
    view_.reset(CreateWebContentsView(this, delegate,
                                      &render_view_host_delegate_view_));
    if (browser_plugin_guest_) {
      view_ = std::make_unique<WebContentsViewGuest>(
          this, browser_plugin_guest_.get(), std::move(view_),
          &render_view_host_delegate_view_);
    }
  }
  CHECK(render_view_host_delegate_view_);
  CHECK(view_.get());

  gfx::Size initial_size = params.initial_size;
  view_->CreateView(initial_size, params.context);

#if BUILDFLAG(ENABLE_PLUGINS)
  plugin_content_origin_whitelist_.reset(
      new PluginContentOriginWhitelist(this));
#endif

  registrar_.Add(this,
                 NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED,
                 NotificationService::AllBrowserContextsAndSources());

  screen_orientation_provider_.reset(new ScreenOrientationProvider(this));

  manifest_manager_host_.reset(new ManifestManagerHost(this));

#if defined(OS_ANDROID)
  date_time_chooser_.reset(new DateTimeChooserAndroid());
#endif

  if (browser_plugin_guest_)
    browser_plugin_guest_->Init();

  for (size_t i = 0; i < g_created_callbacks.Get().size(); i++)
    g_created_callbacks.Get().at(i).Run(this);

  if (params.renderer_initiated_creation) {
    GetRenderViewHost()->GetWidget()->set_renderer_initialized(true);
    GetRenderViewHost()->DispatchRenderViewCreated();
    GetRenderManager()->current_frame_host()->SetRenderFrameCreated(true);
  }

  if (params.desired_renderer_state ==
      CreateParams::kInitializeAndWarmupRendererProcess) {
    if (!GetRenderManager()->current_frame_host()->IsRenderFrameLive()) {
      GetRenderManager()->InitRenderView(GetRenderViewHost(), nullptr);
    }
  }

  NotifySwappedFromRenderManager(
      nullptr, GetRenderManager()->current_frame_host(), true);
}
