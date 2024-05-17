bool ExtensionInstallDialogView::Cancel() {
  if (handled_result_)
    return true;

  handled_result_ = true;
  UpdateInstallResultHistogram(false);
  if (sampling_event_)
    sampling_event_->CreateUserDecisionEvent(ExperienceSamplingEvent::kDeny);
  delegate_->InstallUIAbort(true);
  return true;
}
