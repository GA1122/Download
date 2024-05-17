RenderViewImpl::RenderViewImpl(
    CompositorDependencies* compositor_deps,
    const mojom::CreateViewParams& params,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : RenderWidget(params.view_id,
                   compositor_deps,
                   blink::kWebPopupTypeNone,
                   params.initial_size.screen_info,
                   params.swapped_out,
                   params.hidden,
                   params.never_visible,
                   task_runner),
      webkit_preferences_(params.web_preferences),
      send_content_state_immediately_(false),
      send_preferred_size_changes_(false),
      navigation_gesture_(NavigationGestureUnknown),
      history_list_offset_(-1),
      history_list_length_(0),
      frames_in_progress_(0),
      target_url_status_(TARGET_NONE),
      uses_temporary_zoom_level_(false),
#if defined(OS_ANDROID)
      top_controls_constraints_(BROWSER_CONTROLS_STATE_BOTH),
#endif
      browser_controls_shrink_blink_size_(false),
      top_controls_height_(0.f),
      bottom_controls_height_(0.f),
      webview_(nullptr),
      page_zoom_level_(params.page_zoom_level),
      main_render_frame_(nullptr),
      frame_widget_(nullptr),
      speech_recognition_dispatcher_(nullptr),
#if defined(OS_ANDROID)
      was_created_by_renderer_(false),
#endif
      enumeration_completion_id_(0),
      session_storage_namespace_id_(params.session_storage_namespace_id),
      renderer_wide_named_frame_lookup_(false),
      weak_ptr_factory_(this) {
  GetWidget()->set_owner_delegate(this);
}
