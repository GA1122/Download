void RenderFrameHostImpl::SetNetworkFactoryForTesting(
    const CreateNetworkFactoryCallback& create_network_factory_callback) {
  DCHECK(!BrowserThread::IsThreadInitialized(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(create_network_factory_callback.is_null() ||
         GetCreateNetworkFactoryCallbackForRenderFrame().is_null())
      << "It is not expected that this is called with non-null callback when "
      << "another overriding callback is already set.";
  GetCreateNetworkFactoryCallbackForRenderFrame() =
      create_network_factory_callback;
}