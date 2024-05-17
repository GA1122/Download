bool ExtensionInstallDialogView::Accept() {
  DCHECK(!handled_result_);

  handled_result_ = true;
  UpdateInstallResultHistogram(true);
  if (sampling_event_)
    sampling_event_->CreateUserDecisionEvent(ExperienceSamplingEvent::kProceed);
  base::ResetAndReturn(&done_callback_)
      .Run(ExtensionInstallPrompt::Result::ACCEPTED);
  return true;
}
