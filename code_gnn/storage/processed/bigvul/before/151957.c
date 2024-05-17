GetCreateNetworkFactoryCallbackForRenderFrame() {
  static base::NoDestructor<RenderFrameHostImpl::CreateNetworkFactoryCallback>
      s_callback;
  return *s_callback;
}
