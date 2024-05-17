void HTMLFormElement::prepareForSubmission(
    Event* event,
    HTMLFormControlElement* submitButton) {
  LocalFrame* frame = document().frame();
  if (!frame || m_isSubmitting || m_inUserJSSubmitEvent)
    return;

  if (document().isSandboxed(SandboxForms)) {
    document().addConsoleMessage(ConsoleMessage::create(
        SecurityMessageSource, ErrorMessageLevel,
        "Blocked form submission to '" + m_attributes.action() +
            "' because the form's frame is sandboxed and the 'allow-forms' "
            "permission is not set."));
    return;
  }

  bool skipValidation = !document().page() || noValidate();
  DCHECK(event);
  if (submitButton && submitButton->formNoValidate())
    skipValidation = true;

  UseCounter::count(document(), UseCounter::FormSubmissionStarted);
  if (!skipValidation && !validateInteractively())
    return;

  bool shouldSubmit;
  {
    AutoReset<bool> submitEventHandlerScope(&m_inUserJSSubmitEvent, true);
    frame->loader().client()->dispatchWillSendSubmitEvent(this);
    shouldSubmit =
        dispatchEvent(Event::createCancelableBubble(EventTypeNames::submit)) ==
        DispatchEventResult::NotCanceled;
  }
  if (shouldSubmit) {
    m_plannedNavigation = nullptr;
    submit(event, submitButton);
  }
  if (!m_plannedNavigation)
    return;
  AutoReset<bool> submitScope(&m_isSubmitting, true);
  scheduleFormSubmission(m_plannedNavigation);
  m_plannedNavigation = nullptr;
}
