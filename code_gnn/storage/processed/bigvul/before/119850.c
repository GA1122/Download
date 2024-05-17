int WebContentsImpl::DownloadImage(const GURL& url,
                                   bool is_favicon,
                                   uint32_t preferred_image_size,
                                   uint32_t max_image_size,
                                   const ImageDownloadCallback& callback) {
  RenderViewHost* host = GetRenderViewHost();
  int id = StartDownload(
      host, url, is_favicon, preferred_image_size, max_image_size);
  image_download_map_[id] = callback;
  return id;
}
