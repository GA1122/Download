void PageInfoBubbleView::LinkClicked(views::Link* source, int event_flags) {
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::BindOnce(&PageInfoBubbleView::HandleLinkClickedAsync,
                     weak_factory_.GetWeakPtr(), source));
}
