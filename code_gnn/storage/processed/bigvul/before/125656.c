void RenderViewHostImpl::OnDidZoomURL(double zoom_level,
                                      bool remember,
                                      const GURL& url) {
  HostZoomMapImpl* host_zoom_map = static_cast<HostZoomMapImpl*>(
      HostZoomMap::GetForBrowserContext(GetProcess()->GetBrowserContext()));
  if (remember) {
    host_zoom_map->SetZoomLevel(net::GetHostOrSpecFromURL(url), zoom_level);
  } else {
    host_zoom_map->SetTemporaryZoomLevel(
        GetProcess()->GetID(), GetRoutingID(), zoom_level);
  }
}
