AutomationMouseEventProcessor::AutomationMouseEventProcessor(
    RenderViewHost* render_view_host,
    const AutomationMouseEvent& event,
    const CompletionCallback& completion_callback,
    const ErrorCallback& error_callback)
    : RenderViewHostObserver(render_view_host),
      completion_callback_(completion_callback),
      error_callback_(error_callback),
      has_point_(false) {
  registrar_.Add(this, chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN,
                 content::NotificationService::AllSources());
  Send(new AutomationMsg_ProcessMouseEvent(routing_id(), event));
}
