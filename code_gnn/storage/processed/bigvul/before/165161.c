void HTMLFormElement::SubmitImplicitly(Event* event,
                                       bool from_implicit_submission_trigger) {
  int submission_trigger_count = 0;
  bool seen_default_button = false;
  for (const auto& element : ListedElements()) {
    if (!element->IsFormControlElement())
      continue;
    HTMLFormControlElement* control = ToHTMLFormControlElement(element);
    if (!seen_default_button && control->CanBeSuccessfulSubmitButton()) {
      if (from_implicit_submission_trigger)
        seen_default_button = true;
      if (control->IsSuccessfulSubmitButton()) {
        control->DispatchSimulatedClick(event);
        return;
      }
      if (from_implicit_submission_trigger) {
        return;
      }
    } else if (control->CanTriggerImplicitSubmission()) {
      ++submission_trigger_count;
    }
  }
  if (from_implicit_submission_trigger && submission_trigger_count == 1)
    PrepareForSubmission(event, nullptr);
}
