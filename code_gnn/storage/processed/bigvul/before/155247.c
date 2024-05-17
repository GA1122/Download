void HTMLFormElement::submit(Event* event,
                             HTMLFormControlElement* submitButton) {
  FrameView* view = document().view();
  LocalFrame* frame = document().frame();
  if (!view || !frame || !frame->page())
    return;

  if (!isConnected()) {
    document().addConsoleMessage(ConsoleMessage::create(
        JSMessageSource, WarningMessageLevel,
        "Form submission canceled because the form is not connected"));
    return;
  }

  if (m_isSubmitting)
    return;

  EventQueueScope scopeForDialogClose;
  AutoReset<bool> submitScope(&m_isSubmitting, true);

  if (event && !submitButton) {
    for (const auto& associatedElement : associatedElements()) {
      if (!associatedElement->isFormControlElement())
        continue;
      HTMLFormControlElement* control =
          toHTMLFormControlElement(associatedElement);
      DCHECK(!control->isActivatedSubmit());
      if (control->isSuccessfulSubmitButton()) {
        submitButton = control;
        break;
      }
    }
  }

  FormSubmission* formSubmission =
      FormSubmission::create(this, m_attributes, event, submitButton);
  if (formSubmission->method() == FormSubmission::DialogMethod) {
    submitDialog(formSubmission);
  } else if (m_inUserJSSubmitEvent) {
    m_plannedNavigation = formSubmission;
  } else {
    scheduleFormSubmission(formSubmission);
  }
}
