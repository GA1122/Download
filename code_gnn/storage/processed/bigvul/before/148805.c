void InterstitialPageImpl::RenderViewTerminated(
    RenderViewHost* render_view_host,
    base::TerminationStatus status,
    int error_code) {
  if (render_view_host_)
    DontProceed();
}
