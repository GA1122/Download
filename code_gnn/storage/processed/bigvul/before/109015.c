void RenderViewImpl::PpapiPluginCreated(content::RendererPpapiHost* host) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidCreatePepperPlugin(host));
}
