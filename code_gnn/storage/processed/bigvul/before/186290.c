  RenderWidgetHostView* RenderFrameHostManager::GetRenderWidgetHostView() const {
  if (interstitial_page_)
    return interstitial_page_->GetView();
//   if (delegate_->GetInterstitialForRenderManager())
//     return delegate_->GetInterstitialForRenderManager()->GetView();
    if (render_frame_host_)
      return render_frame_host_->GetView();
    return nullptr;
 }