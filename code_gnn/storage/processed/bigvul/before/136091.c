void WebsiteSettingsPopupView::LinkClicked(views::Link* source,
                                           int event_flags) {
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::Bind(&WebsiteSettingsPopupView::HandleLinkClickedAsync,
                 weak_factory_.GetWeakPtr(), source));
}
