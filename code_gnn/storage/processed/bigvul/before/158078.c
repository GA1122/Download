void LocalFrameClientImpl::DispatchDidStartProvisionalLoad(
    DocumentLoader* loader,
    ResourceRequest& request) {
  if (web_frame_->Client()) {
    WrappedResourceRequest wrapped_request(request);
    web_frame_->Client()->DidStartProvisionalLoad(
        WebDocumentLoaderImpl::FromDocumentLoader(loader), wrapped_request);
  }
  if (WebDevToolsAgentImpl* dev_tools = DevToolsAgent())
    dev_tools->DidStartProvisionalLoad(web_frame_->GetFrame());
  virtual_time_pauser_.PauseVirtualTime();
}
