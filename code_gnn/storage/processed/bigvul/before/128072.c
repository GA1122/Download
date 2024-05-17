void SynchronousCompositorImpl::DeliverMessages() {
  ScopedVector<IPC::Message> messages;
  output_surface_->GetMessagesToDeliver(&messages);
  RenderProcessHost* rph = rwhva_->GetRenderWidgetHost()->GetProcess();
  for (ScopedVector<IPC::Message>::const_iterator i = messages.begin();
       i != messages.end();
       ++i) {
    rph->OnMessageReceived(**i);
  }
}
