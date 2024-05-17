void WebContentsImpl::OnPageScaleFactorChanged(RenderViewHostImpl* source,
                                               float page_scale_factor) {
#if !defined(OS_ANDROID)
  bool is_one = page_scale_factor == 1.f;
  if (is_one != page_scale_factor_is_one_) {
    page_scale_factor_is_one_ = is_one;

    HostZoomMapImpl* host_zoom_map =
        static_cast<HostZoomMapImpl*>(HostZoomMap::GetForWebContents(this));

    if (host_zoom_map) {
      host_zoom_map->SetPageScaleFactorIsOneForView(
          source->GetProcess()->GetID(), source->GetRoutingID(),
          page_scale_factor_is_one_);
    }
  }
#endif   

  for (auto& observer : observers_)
    observer.OnPageScaleFactorChanged(page_scale_factor);
}
