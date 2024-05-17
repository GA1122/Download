void InterstitialPageImpl::UpdateDeviceScaleFactor(double device_scale_factor) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents_);
  if (!web_contents_impl)
    return;

  web_contents_impl->UpdateDeviceScaleFactor(device_scale_factor);
}
