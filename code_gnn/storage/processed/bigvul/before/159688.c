void OnDefaultProtocolClientWorkerFinished(
    const GURL& escaped_url,
    int render_process_host_id,
    int render_view_routing_id,
    bool prompt_user,
    ui::PageTransition page_transition,
    bool has_user_gesture,
    ExternalProtocolHandler::Delegate* delegate,
    shell_integration::DefaultWebClientState state) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (delegate)
    delegate->FinishedProcessingCheck();

  if (state == shell_integration::IS_DEFAULT) {
    if (delegate)
      delegate->BlockRequest();
    return;
  }

  if (prompt_user) {
    RunExternalProtocolDialogWithDelegate(
        escaped_url, render_process_host_id, render_view_routing_id,
        page_transition, has_user_gesture, delegate);
    return;
  }

  LaunchUrlWithoutSecurityCheckWithDelegate(escaped_url, render_process_host_id,
                                            render_view_routing_id, delegate);
}
