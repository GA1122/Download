void PasswordAutofillAgent::OnFrameDetached() {
  if (render_frame()->GetWebFrame()->Parent() && browser_has_form_to_process_) {
    DCHECK(FrameCanAccessPasswordManager());
    GetPasswordManagerDriver()->SameDocumentNavigation(
        SubmissionIndicatorEvent::FRAME_DETACHED);
  }
  CleanupOnDocumentShutdown();
}
