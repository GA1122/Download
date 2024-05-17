void RenderFrameImpl::PepperPluginCreated(RendererPpapiHost* host) {
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_,
                    DidCreatePepperPlugin(host));
  if (host->GetPluginName() == kFlashPluginName) {
    RenderThread::Get()->RecordAction(
        base::UserMetricsAction("FrameLoadWithFlash"));
  }
}
