void PasswordAutofillAgent::OnInferredFormSubmission(SubmissionSource source) {
  if (source == SubmissionSource::FRAME_DETACHED) {
    OnFrameDetached();
  } else {
    SubmissionIndicatorEvent event = ToSubmissionIndicatorEvent(source);
    if (event == SubmissionIndicatorEvent::NONE)
      return;
    FireSubmissionIfFormDisappear(event);
  }
}
