void RunExternalProtocolDialogWithDelegate(
    const GURL& url,
    int render_process_host_id,
    int routing_id,
    ui::PageTransition page_transition,
    bool has_user_gesture,
    ExternalProtocolHandler::Delegate* delegate) {
  if (delegate) {
    delegate->RunExternalProtocolDialog(url, render_process_host_id, routing_id,
                                        page_transition, has_user_gesture);
    return;
  }
  ExternalProtocolHandler::RunExternalProtocolDialog(
      url, render_process_host_id, routing_id, page_transition,
      has_user_gesture);
}
