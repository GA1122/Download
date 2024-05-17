RenderWidgetHostView* RenderViewHostManager::GetRenderWidgetHostView() const {
  if (interstitial_page_)
    return interstitial_page_->GetView();
  if (!render_view_host_)
    return NULL;
  return render_view_host_->GetView();
}
