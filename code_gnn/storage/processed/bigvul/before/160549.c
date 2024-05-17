void RenderFrameImpl::BindWidget(mojom::WidgetRequest request) {
  GetRenderWidget()->SetWidgetBinding(std::move(request));
}
