TestRenderFrame::TestRenderFrame(RenderFrameImpl::CreateParams params)
    : RenderFrameImpl(std::move(params)),
      mock_frame_host_(std::make_unique<MockFrameHost>()) {
  MockRenderThread* mock_render_thread =
      static_cast<MockRenderThread*>(RenderThread::Get());
  mock_frame_host_->PassLastInterfaceProviderRequest(
      mock_render_thread->TakeInitialInterfaceProviderRequestForFrame(
          params.routing_id));
  mock_frame_host_->PassLastDocumentInterfaceBrokerRequest(
      mock_render_thread->TakeInitialDocumentInterfaceBrokerRequestForFrame(
          params.routing_id));
}
