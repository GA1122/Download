void RenderViewImpl::PpapiPluginCreated(RendererPpapiHost* host) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidCreatePepperPlugin(host));
}
