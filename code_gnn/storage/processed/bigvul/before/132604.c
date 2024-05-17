void WebKitTestController::RenderViewCreated(RenderViewHost* render_view_host) {
  DCHECK(CalledOnValidThread());
  if (render_view_host->GetProcess()->GetHandle() != base::kNullProcessHandle)
    current_pid_ = base::GetProcId(render_view_host->GetProcess()->GetHandle());
  if (!send_configuration_to_next_host_)
    return;
  send_configuration_to_next_host_ = false;
  SendTestConfiguration();
}
