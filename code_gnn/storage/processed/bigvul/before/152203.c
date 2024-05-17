void RenderFrameImpl::BindWidget(mojom::WidgetRequest request) {
  GetLocalRootRenderWidget()->SetWidgetBinding(std::move(request));
}
