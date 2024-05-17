 void OffscreenCanvasFrameReceiverImpl::Create(
    blink::mojom::OffscreenCanvasFrameReceiverRequest request) {
  mojo::MakeStrongBinding(base::MakeUnique<OffscreenCanvasFrameReceiverImpl>(),
                          std::move(request));
}
