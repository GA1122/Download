SynchronousCompositorImpl::SynchronousCompositorImpl(
    RenderWidgetHostViewAndroid* rwhva,
    SynchronousCompositorClient* client)
    : rwhva_(rwhva),
      routing_id_(rwhva_->GetRenderWidgetHost()->GetRoutingID()),
      compositor_client_(client),
      output_surface_(nullptr),
      begin_frame_source_(nullptr),
      synchronous_input_handler_proxy_(nullptr),
      registered_with_client_(false),
      is_active_(true),
      renderer_needs_begin_frames_(false),
      need_animate_input_(false),
      weak_ptr_factory_(this) {
  DCHECK_NE(routing_id_, MSG_ROUTING_NONE);

  int process_id = rwhva_->GetRenderWidgetHost()->GetProcess()->GetID();
  if (g_process_id == ChildProcessHost::kInvalidUniqueID) {
    g_process_id = process_id;
  } else {
    DCHECK_EQ(g_process_id, process_id);   
  }

  SynchronousCompositorRegistryInProc::GetInstance()->RegisterCompositor(
      routing_id_, this);
}
