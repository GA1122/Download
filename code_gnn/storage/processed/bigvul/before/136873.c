void HTMLInputElement::DefaultEventHandler(Event* evt) {
  if (evt->IsMouseEvent() && evt->type() == EventTypeNames::click &&
      ToMouseEvent(evt)->button() ==
          static_cast<short>(WebPointerProperties::Button::kLeft)) {
    input_type_view_->HandleClickEvent(ToMouseEvent(evt));
    if (evt->DefaultHandled())
      return;
  }

  if (evt->IsKeyboardEvent() && evt->type() == EventTypeNames::keydown) {
    input_type_view_->HandleKeydownEvent(ToKeyboardEvent(evt));
    if (evt->DefaultHandled())
      return;
  }

  bool call_base_class_early =
      IsTextField() && (evt->type() == EventTypeNames::keydown ||
                        evt->type() == EventTypeNames::keypress);
  if (call_base_class_early) {
    TextControlElement::DefaultEventHandler(evt);
    if (evt->DefaultHandled())
      return;
  }

  if (evt->type() == EventTypeNames::DOMActivate) {
    input_type_view_->HandleDOMActivateEvent(evt);
    if (evt->DefaultHandled())
      return;
  }

  if (evt->IsKeyboardEvent() && evt->type() == EventTypeNames::keypress) {
    input_type_view_->HandleKeypressEvent(ToKeyboardEvent(evt));
    if (evt->DefaultHandled())
      return;
  }

  if (evt->IsKeyboardEvent() && evt->type() == EventTypeNames::keyup) {
    input_type_view_->HandleKeyupEvent(ToKeyboardEvent(evt));
    if (evt->DefaultHandled())
      return;
  }

  if (input_type_view_->ShouldSubmitImplicitly(evt)) {
    if (type() == InputTypeNames::search) {
      GetDocument()
          .GetTaskRunner(TaskType::kUserInteraction)
          ->PostTask(FROM_HERE, WTF::Bind(&HTMLInputElement::OnSearch,
                                          WrapPersistent(this)));
    }
    DispatchFormControlChangeEvent();

    HTMLFormElement* form_for_submission =
        input_type_view_->FormForSubmission();
    if (form_for_submission) {
      form_for_submission->SubmitImplicitly(evt,
                                            CanTriggerImplicitSubmission());
    }
    evt->SetDefaultHandled();
    return;
  }

  if (evt->IsBeforeTextInsertedEvent()) {
    input_type_view_->HandleBeforeTextInsertedEvent(
        static_cast<BeforeTextInsertedEvent*>(evt));
  }

  if (evt->IsMouseEvent() && evt->type() == EventTypeNames::mousedown) {
    input_type_view_->HandleMouseDownEvent(ToMouseEvent(evt));
    if (evt->DefaultHandled())
      return;
  }

  input_type_view_->ForwardEvent(evt);

  if (!call_base_class_early && !evt->DefaultHandled())
    TextControlElement::DefaultEventHandler(evt);
}
