RenderFrameImpl::RenderFrameImpl(CreateParams params)
    : frame_(nullptr),
      is_main_frame_(true),
      unique_name_frame_adapter_(this),
      unique_name_helper_(&unique_name_frame_adapter_),
      in_frame_tree_(false),
      render_view_(params.render_view),
      routing_id_(params.routing_id),
      proxy_routing_id_(MSG_ROUTING_NONE),
#if BUILDFLAG(ENABLE_PLUGINS)
      plugin_power_saver_helper_(nullptr),
#endif
      cookie_jar_(this),
      selection_text_offset_(0),
      selection_range_(gfx::Range::InvalidRange()),
      handling_select_range_(false),
      web_user_media_client_(nullptr),
      push_messaging_client_(nullptr),
      render_accessibility_(nullptr),
      previews_state_(PREVIEWS_UNSPECIFIED),
      effective_connection_type_(
          blink::WebEffectiveConnectionType::kTypeUnknown),
      is_pasting_(false),
      suppress_further_dialogs_(false),
      blame_context_(nullptr),
#if BUILDFLAG(ENABLE_PLUGINS)
      focused_pepper_plugin_(nullptr),
      pepper_last_mouse_event_target_(nullptr),
#endif
      autoplay_configuration_binding_(this),
      frame_binding_(this),
      host_zoom_binding_(this),
      frame_bindings_control_binding_(this),
      frame_navigation_control_binding_(this),
      fullscreen_binding_(this),
      navigation_client_impl_(nullptr),
      has_accessed_initial_document_(false),
      media_factory_(this,
                     base::Bind(&RenderFrameImpl::RequestOverlayRoutingToken,
                                base::Unretained(this))),
      input_target_client_impl_(this),
      devtools_frame_token_(params.devtools_frame_token),
      weak_factory_(this) {
  CHECK(params.interface_provider.is_bound());
  remote_interfaces_.Bind(std::move(params.interface_provider));
  blink_interface_registry_.reset(new BlinkInterfaceRegistryImpl(
      registry_.GetWeakPtr(), associated_interfaces_.GetWeakPtr()));

  media_factory_.SetupMojo();

  std::pair<RoutingIDFrameMap::iterator, bool> result =
      g_routing_id_frame_map.Get().insert(std::make_pair(routing_id_, this));
  CHECK(result.second) << "Inserting a duplicate item.";

#if defined(OS_ANDROID)
  new GinJavaBridgeDispatcher(this);
#endif

#if BUILDFLAG(ENABLE_PLUGINS)
  plugin_power_saver_helper_ = new PluginPowerSaverHelper(this);
#endif

  manifest_manager_ = std::make_unique<ManifestManager>(this);
  if (IsMainFrame()) {
    new ManifestChangeNotifier(this);
  }
}
