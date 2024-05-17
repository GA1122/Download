void WebContentsImpl::RenderViewTerminated(RenderViewHost* rvh,
                                           base::TerminationStatus status,
                                           int error_code) {
  if (rvh != GetRenderViewHost()) {
    return;
  }

  if (IsFullscreenForCurrentTab())
    ExitFullscreenMode(false);

  CancelActiveAndPendingDialogs();

  if (delegate_)
    delegate_->HideValidationMessage(this);

  audio_stream_monitor_.RenderProcessGone(rvh->GetProcess()->GetID());

  ResetLoadProgressState();
  NotifyDisconnected();
  SetIsCrashed(status, error_code);

  for (auto& observer : observers_)
    observer.RenderProcessGone(GetCrashedStatus());
}
