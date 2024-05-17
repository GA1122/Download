AudioRendererHost::DoGetOutputControllers() const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  RenderProcessHost::AudioOutputControllerList controllers;
  for (const auto& delegate : delegates_)
    controllers.push_back(delegate->GetController());

  return controllers;
}
