int WebContentsImpl::DownloadImage(
    const GURL& url,
    bool is_favicon,
    uint32_t max_bitmap_size,
    bool bypass_cache,
    const WebContents::ImageDownloadCallback& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  static int next_image_download_id = 0;
  const content::mojom::ImageDownloaderPtr& mojo_image_downloader =
      GetMainFrame()->GetMojoImageDownloader();
  const int download_id = ++next_image_download_id;
  if (!mojo_image_downloader) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::Bind(&WebContentsImpl::OnDidDownloadImage,
                   weak_factory_.GetWeakPtr(), callback, download_id, url, 400,
                   std::vector<SkBitmap>(), std::vector<gfx::Size>()));
    return download_id;
  }

  mojo_image_downloader->DownloadImage(
      url, is_favicon, max_bitmap_size, bypass_cache,
      base::Bind(&WebContentsImpl::OnDidDownloadImage,
                 weak_factory_.GetWeakPtr(), callback, download_id, url));
  return download_id;
}
