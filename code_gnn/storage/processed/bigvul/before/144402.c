bool ExtensionInstallDialogView::Accept() {
  DCHECK(!handled_result_);

  handled_result_ = true;
  UpdateInstallResultHistogram(true);
  if (sampling_event_)
    sampling_event_->CreateUserDecisionEvent(ExperienceSamplingEvent::kProceed);
  delegate_->InstallUIProceed();
  return true;
 }
