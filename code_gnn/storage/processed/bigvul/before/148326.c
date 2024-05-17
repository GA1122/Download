void WebContentsImpl::CreateNewWidget(int32_t render_process_id,
                                      int32_t route_id,
                                      bool is_fullscreen,
                                      blink::WebPopupType popup_type) {
  RenderProcessHost* process = RenderProcessHost::FromID(render_process_id);
  if (!HasMatchingProcess(&frame_tree_, render_process_id)) {
    base::ProcessHandle process_handle = process->GetHandle();
    if (process_handle != base::kNullProcessHandle) {
      RecordAction(
          base::UserMetricsAction("Terminate_ProcessMismatch_CreateNewWidget"));
      process->Shutdown(RESULT_CODE_KILLED, false);
    }
    return;
  }

  RenderWidgetHostImpl* widget_host =
      new RenderWidgetHostImpl(this, process, route_id, IsHidden());

  RenderWidgetHostViewBase* widget_view =
      static_cast<RenderWidgetHostViewBase*>(
          view_->CreateViewForPopupWidget(widget_host));
  if (!widget_view)
    return;
  if (!is_fullscreen) {
    widget_view->SetPopupType(popup_type);
  }
  pending_widget_views_[std::make_pair(render_process_id, route_id)] =
      widget_view;

#if defined(OS_MACOSX)
  base::mac::NSObjectRetain(widget_view->GetNativeView());
#endif
}
