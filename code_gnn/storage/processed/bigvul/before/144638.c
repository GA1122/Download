void WebContentsImpl::RenderViewTerminated(RenderViewHost* rvh,
                                           base::TerminationStatus status,
                                           int error_code) {
  if (rvh != GetRenderViewHost()) {
    return;
  }

  if (IsFullscreenForCurrentTab(GetRenderViewHost()->GetWidget()))
    ExitFullscreenMode(false);

  CancelActiveAndPendingDialogs();

  if (delegate_)
    delegate_->HideValidationMessage(this);

  SetIsLoading(false, true, nullptr);
  NotifyDisconnected();
  SetIsCrashed(status, error_code);

  ResetLoadProgressState();

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    RenderProcessGone(GetCrashedStatus()));
}
