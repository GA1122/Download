void WebContentsImpl::OnDidDownloadImage(
    const ImageDownloadCallback& callback,
    int id,
    const GURL& image_url,
    int32_t http_status_code,
    const std::vector<SkBitmap>& images,
    const std::vector<gfx::Size>& original_image_sizes) {
  callback.Run(id, http_status_code, image_url, images, original_image_sizes);
}
