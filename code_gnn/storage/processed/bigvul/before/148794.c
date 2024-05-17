void InterstitialPageImpl::Observe(
    int type,
    const NotificationSource& source,
    const NotificationDetails& details) {
  switch (type) {
    case NOTIFICATION_NAV_ENTRY_PENDING:
      Disable();
      TakeActionOnResourceDispatcher(CANCEL);
      break;
    case NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED:
      if (action_taken_ == NO_ACTION) {
        RenderViewHost* rvh =
            RenderViewHost::From(Source<RenderWidgetHost>(source).ptr());
        DCHECK(rvh->GetProcess()->GetID() == original_child_id_ &&
               rvh->GetRoutingID() == original_rvh_id_);
        TakeActionOnResourceDispatcher(CANCEL);
      }
      break;
    default:
      NOTREACHED();
  }
}
