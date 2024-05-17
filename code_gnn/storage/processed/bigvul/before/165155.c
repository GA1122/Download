void HTMLFormElement::PrepareForSubmission(
    Event* event,
    HTMLFormControlElement* submit_button) {
  LocalFrame* frame = GetDocument().GetFrame();
  if (!frame || is_submitting_ || in_user_js_submit_event_)
    return;

  if (!isConnected()) {
    GetDocument().AddConsoleMessage(ConsoleMessage::Create(
        kJSMessageSource, kWarningMessageLevel,
        "Form submission canceled because the form is not connected"));
    return;
  }

  if (GetDocument().IsSandboxed(kSandboxForms)) {
    GetDocument().AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, kErrorMessageLevel,
        "Blocked form submission to '" + attributes_.Action() +
            "' because the form's frame is sandboxed and the 'allow-forms' "
            "permission is not set."));
    return;
  }

  for (const auto& element : ListedElements()) {
    if (element->IsFormControlElement() &&
        ToHTMLFormControlElement(element)->BlocksFormSubmission()) {
      UseCounter::Count(GetDocument(),
                        WebFeature::kFormSubmittedWithUnclosedFormControl);
      if (RuntimeEnabledFeatures::UnclosedFormControlIsInvalidEnabled()) {
        String tag_name = ToHTMLFormControlElement(element)->tagName();
        GetDocument().AddConsoleMessage(ConsoleMessage::Create(
            kSecurityMessageSource, kErrorMessageLevel,
            "Form submission failed, as the <" + tag_name +
                "> element named "
                "'" +
                element->GetName() +
                "' was implicitly closed by reaching "
                "the end of the file. Please add an explicit end tag "
                "('</" +
                tag_name + ">')"));
        DispatchEvent(Event::Create(EventTypeNames::error));
        return;
      }
    }
  }

  bool skip_validation = !GetDocument().GetPage() || NoValidate();
  DCHECK(event);
  if (submit_button && submit_button->FormNoValidate())
    skip_validation = true;

  UseCounter::Count(GetDocument(), WebFeature::kFormSubmissionStarted);
  if (!skip_validation && !ValidateInteractively())
    return;

  bool should_submit;
  {
    AutoReset<bool> submit_event_handler_scope(&in_user_js_submit_event_, true);
    frame->Client()->DispatchWillSendSubmitEvent(this);
    should_submit =
        DispatchEvent(Event::CreateCancelableBubble(EventTypeNames::submit)) ==
        DispatchEventResult::kNotCanceled;
  }
  if (should_submit) {
    planned_navigation_ = nullptr;
    Submit(event, submit_button);
  }
  if (!planned_navigation_)
    return;
  AutoReset<bool> submit_scope(&is_submitting_, true);
  ScheduleFormSubmission(planned_navigation_);
  planned_navigation_ = nullptr;
}
