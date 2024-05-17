void WebBluetoothServiceImpl::OnVisibilityChanged(Visibility visibility) {
  if (visibility == content::Visibility::HIDDEN ||
      visibility == content::Visibility::OCCLUDED) {
    allowed_scan_filters_.clear();
    accept_all_advertisements_ = false;
    scanning_clients_.clear();
  }
}
