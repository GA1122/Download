std::unique_ptr<Renderer> WebMediaPlayerImpl::CreateRenderer() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (always_enable_overlays_)
    EnableOverlay();

  RequestOverlayInfoCB request_overlay_info_cb;
#if defined(OS_ANDROID)
  request_overlay_info_cb = BindToCurrentLoop(
      base::Bind(&WebMediaPlayerImpl::OnOverlayInfoRequested, AsWeakPtr()));
#endif
  return renderer_factory_selector_->GetCurrentFactory()->CreateRenderer(
      media_task_runner_, worker_task_runner_, audio_source_provider_.get(),
      compositor_.get(), request_overlay_info_cb, client_->TargetColorSpace());
}
