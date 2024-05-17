 void RenderFrameDevToolsAgentHost::AttachSession(DevToolsSession* session) {
// bool RenderFrameDevToolsAgentHost::AttachSession(DevToolsSession* session) {
//   if (session->restricted() && !IsFrameHostAllowedForRestrictedSessions())
//     return false;
    session->SetRenderer(frame_host_ ? frame_host_->GetProcess()->GetID()
                                     : ChildProcessHost::kInvalidUniqueID,
                         frame_host_);
 
   protocol::EmulationHandler* emulation_handler =
       new protocol::EmulationHandler();
   session->AddHandler(base::WrapUnique(new protocol::BrowserHandler()));
   session->AddHandler(base::WrapUnique(new protocol::DOMHandler()));
   session->AddHandler(base::WrapUnique(emulation_handler));
   session->AddHandler(base::WrapUnique(new protocol::InputHandler()));
   session->AddHandler(base::WrapUnique(new protocol::InspectorHandler()));
   session->AddHandler(base::WrapUnique(new protocol::IOHandler(
       GetIOContext())));
   session->AddHandler(base::WrapUnique(new protocol::MemoryHandler()));
   session->AddHandler(base::WrapUnique(new protocol::NetworkHandler(GetId())));
   session->AddHandler(base::WrapUnique(new protocol::SchemaHandler()));
   session->AddHandler(base::WrapUnique(new protocol::ServiceWorkerHandler()));
   session->AddHandler(base::WrapUnique(new protocol::StorageHandler()));
   session->AddHandler(
       base::WrapUnique(new protocol::TargetHandler(false  )));
   session->AddHandler(base::WrapUnique(new protocol::TracingHandler(
       protocol::TracingHandler::Renderer,
       frame_tree_node_ ? frame_tree_node_->frame_tree_node_id() : 0,
       GetIOContext())));
   session->AddHandler(
       base::WrapUnique(new protocol::PageHandler(emulation_handler)));
   session->AddHandler(base::WrapUnique(new protocol::SecurityHandler()));
 
   if (EnsureAgent())
     session->AttachToAgent(agent_ptr_);
 
   if (sessions().size() == 1) {
     frame_trace_recorder_.reset(new DevToolsFrameTraceRecorder());
     GrantPolicy();
 #if defined(OS_ANDROID)
      GetWakeLock()->RequestWakeLock();
  #endif
    }
//   return true;
  }