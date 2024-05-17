void ResourceMessageFilter::OnDidZoomURL(const GURL& url,
                                         int zoom_level) {
  ChromeThread::PostTask(ChromeThread::UI, FROM_HERE,
      NewRunnableMethod(this,
                        &ResourceMessageFilter::UpdateHostZoomLevelsOnUIThread,
                        url, zoom_level));
}
