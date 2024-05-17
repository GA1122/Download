void Compositor::IssueExternalBeginFrame(const viz::BeginFrameArgs& args) {
  TRACE_EVENT1("ui", "Compositor::IssueExternalBeginFrame", "args",
               args.AsValue());
  DCHECK(external_begin_frames_enabled_);
  if (context_factory_private_)
    context_factory_private_->IssueExternalBeginFrame(this, args);
}
