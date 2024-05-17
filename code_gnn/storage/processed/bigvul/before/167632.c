  void OnShowCreatedWindow(int pending_widget_routing_id,
                           WindowOpenDisposition disposition,
                           const gfx::Rect& initial_rect,
                           bool user_gesture) {
    content::BrowserThread::PostTask(
        content::BrowserThread::UI, FROM_HERE,
        base::BindOnce(&PendingWidgetMessageFilter::OnReceivedRoutingIDOnUI,
                       this, pending_widget_routing_id));
  }
