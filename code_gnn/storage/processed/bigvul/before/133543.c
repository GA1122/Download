void WebContentsImpl::CreateNewWidget(int render_process_id,
                                      int route_id,
                                      bool is_fullscreen,
                                      blink::WebPopupType popup_type) {
  RenderProcessHost* process = GetRenderProcessHost();
  if (process->GetID() != render_process_id) {
    base::ProcessHandle process_handle =
        RenderProcessHost::FromID(render_process_id)->GetHandle();
    if (process_handle != base::kNullProcessHandle) {
      RecordAction(
          base::UserMetricsAction("Terminate_ProcessMismatch_CreateNewWidget"));
      base::KillProcess(process_handle, content::RESULT_CODE_KILLED, false);
    }
    return;
  }

  RenderWidgetHostImpl* widget_host =
      new RenderWidgetHostImpl(this, process, route_id, IsHidden());
  created_widgets_.insert(widget_host);

  RenderWidgetHostViewPort* widget_view = RenderWidgetHostViewPort::FromRWHV(
      view_->CreateViewForPopupWidget(widget_host));
  if (!widget_view)
    return;
  if (!is_fullscreen) {
    widget_view->SetPopupType(popup_type);
  }
  pending_widget_views_[route_id] = widget_view;

#if defined(OS_MACOSX)
  base::mac::NSObjectRetain(widget_view->GetNativeView());
#endif
}
