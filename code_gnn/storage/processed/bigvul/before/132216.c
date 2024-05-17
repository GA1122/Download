void RenderFrameImpl::Initialize() {
  is_subframe_ = !!frame_->parent();
  is_local_root_ = !frame_->parent() || frame_->parent()->isWebRemoteFrame();

#if defined(ENABLE_PLUGINS)
  new PepperBrowserConnection(this);
#endif
  new SharedWorkerRepository(this);

  if (is_local_root_ && !render_frame_proxy_) {
    devtools_agent_ = new DevToolsAgent(this);
  }

  RegisterMojoServices();

  GetContentClient()->renderer()->RenderFrameCreated(this);
}
