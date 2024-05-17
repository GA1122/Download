void RenderProcessHostImpl::OnProcessLaunchFailed(int error_code) {
  if (deleting_soon_)
    return;

  RendererClosedDetails details{base::TERMINATION_STATUS_LAUNCH_FAILED,
                                error_code};
  ProcessDied(true, &details);
}
