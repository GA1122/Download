void PasswordAutofillAgent::FireSubmissionIfFormDisappear(
    SubmissionIndicatorEvent event) {
  if (!browser_has_form_to_process_)
    return;
  DCHECK(FrameCanAccessPasswordManager());

  WebLocalFrame* frame = render_frame()->GetWebFrame();
  if (event != SubmissionIndicatorEvent::DOM_MUTATION_AFTER_XHR) {
    bool is_last_updated_field_in_form =
        last_updated_form_renderer_id_ != FormData::kNotSetFormRendererId;
    if (is_last_updated_field_in_form) {
      if (IsFormVisible(frame, last_updated_form_renderer_id_))
        return;
    } else {
      if (IsFormControlVisible(frame, last_updated_field_renderer_id_))
        return;
    }
  }
  GetPasswordManagerDriver()->SameDocumentNavigation(event);
  browser_has_form_to_process_ = false;
}
