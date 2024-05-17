void WebContentsImpl::UpdateZoom() {
  RenderWidgetHostImpl* rwh = GetRenderViewHost()->GetWidget();
  if (rwh->GetView())
    rwh->SynchronizeVisualProperties();
}
