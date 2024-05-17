void ExternalProtocolHandler::RunExternalProtocolDialog(
    const GURL& url, int render_process_host_id, int routing_id,
    ui::PageTransition page_transition, bool has_user_gesture) {
  std::unique_ptr<ExternalProtocolDialogDelegate> delegate(
      new ExternalProtocolDialogDelegate(url, render_process_host_id,
                                         routing_id));
  if (delegate->program_name().empty()) {
    return;
  }

  new ExternalProtocolDialog(std::move(delegate), render_process_host_id,
                             routing_id);
}
