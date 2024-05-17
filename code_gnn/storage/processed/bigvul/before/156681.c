  void OnShowCreatedWindow(int pending_widget_routing_id,
                           WindowOpenDisposition disposition,
                           const gfx::Rect& initial_rect,
                           bool user_gesture) {
    base::PostTaskWithTraits(
        FROM_HERE, {content::BrowserThread::UI},
        base::BindOnce(&PendingWidgetMessageFilter::OnReceivedRoutingIDOnUI,
                       this, pending_widget_routing_id));
  }
