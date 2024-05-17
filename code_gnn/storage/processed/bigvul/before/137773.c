void HeadlessWebContentsImpl::RenderProcessExited(
    content::RenderProcessHost* host,
    base::TerminationStatus status,
    int exit_code) {
  DCHECK_EQ(render_process_host_, host);
  for (auto& observer : observers_)
    observer.RenderProcessExited(status, exit_code);
}
