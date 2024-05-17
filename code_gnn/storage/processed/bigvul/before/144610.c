void WebContentsImpl::OnPageScaleFactorChanged(float page_scale_factor) {
  bool is_one = page_scale_factor == 1.f;
  if (is_one != page_scale_factor_is_one_) {
    page_scale_factor_is_one_ = is_one;

    HostZoomMapImpl* host_zoom_map =
        static_cast<HostZoomMapImpl*>(HostZoomMap::GetForWebContents(this));

    if (host_zoom_map && GetRenderProcessHost()) {
      host_zoom_map->SetPageScaleFactorIsOneForView(
          GetRenderProcessHost()->GetID(), GetRoutingID(),
          page_scale_factor_is_one_);
    }
  }

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    OnPageScaleFactorChanged(page_scale_factor));
}
