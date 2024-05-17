void RenderFrameImpl::Initialize() {
  is_main_frame_ = !frame_->Parent();

  GetRenderWidget()->RegisterRenderFrame(this);

  RenderFrameImpl* parent_frame =
      RenderFrameImpl::FromWebFrame(frame_->Parent());
  if (parent_frame) {
    previews_state_ = parent_frame->GetPreviewsState();
    effective_connection_type_ = parent_frame->GetEffectiveConnectionType();
  }

  bool is_tracing_rail = false;
  bool is_tracing_navigation = false;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED("navigation", &is_tracing_navigation);
  TRACE_EVENT_CATEGORY_GROUP_ENABLED("rail", &is_tracing_rail);
  if (is_tracing_rail || is_tracing_navigation) {
    int parent_id = RenderFrame::GetRoutingIdForWebFrame(frame_->Parent());
    TRACE_EVENT2("navigation,rail", "RenderFrameImpl::Initialize",
                 "id", routing_id_,
                 "parent", parent_id);
  }

#if BUILDFLAG(ENABLE_PLUGINS)
  new PepperBrowserConnection(this);
#endif
  shared_worker_repository_ =
      std::make_unique<SharedWorkerRepository>(GetInterfaceProvider());
  GetWebFrame()->SetSharedWorkerRepositoryClient(
      shared_worker_repository_.get());

  RegisterMojoInterfaces();

  GetContentClient()->renderer()->RenderFrameCreated(this);

  RenderThreadImpl* render_thread = RenderThreadImpl::current();
  InputHandlerManager* input_handler_manager =
      render_thread ? render_thread->input_handler_manager() : nullptr;
  if (input_handler_manager) {
    DCHECK(render_view_->HasAddedInputHandler());
    input_handler_manager->RegisterAssociatedRenderFrameRoutingID(
        GetRoutingID(), render_view_->GetRoutingID());
  }

  if (auto* factory = AudioOutputIPCFactory::get())
    factory->MaybeRegisterRemoteFactory(GetRoutingID(), GetRemoteInterfaces());

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDomAutomationController))
    enabled_bindings_ |= BINDINGS_POLICY_DOM_AUTOMATION;
  if (command_line.HasSwitch(switches::kStatsCollectionController))
    enabled_bindings_ |= BINDINGS_POLICY_STATS_COLLECTION;
}
