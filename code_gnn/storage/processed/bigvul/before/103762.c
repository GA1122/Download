  RenderViewZoomer(const GURL& url, double zoom_level)
      : zoom_level_(zoom_level) {
    host_ = net::GetHostOrSpecFromURL(url);
  }
