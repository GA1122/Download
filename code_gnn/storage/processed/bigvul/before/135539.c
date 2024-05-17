void Editor::CountEvent(ExecutionContext* execution_context,
                        const Event* event) {
  if (!execution_context)
    return;

  if (event->type() == EventTypeNames::textInput) {
    CountEditingEvent(execution_context, event,
                      WebFeature::kTextInputEventOnInput,
                      WebFeature::kTextInputEventOnTextArea,
                      WebFeature::kTextInputEventOnContentEditable,
                      WebFeature::kTextInputEventOnNotNode);
    return;
  }

  if (event->type() == EventTypeNames::webkitBeforeTextInserted) {
    CountEditingEvent(execution_context, event,
                      WebFeature::kWebkitBeforeTextInsertedOnInput,
                      WebFeature::kWebkitBeforeTextInsertedOnTextArea,
                      WebFeature::kWebkitBeforeTextInsertedOnContentEditable,
                      WebFeature::kWebkitBeforeTextInsertedOnNotNode);
    return;
  }

  if (event->type() == EventTypeNames::webkitEditableContentChanged) {
    CountEditingEvent(
        execution_context, event,
        WebFeature::kWebkitEditableContentChangedOnInput,
        WebFeature::kWebkitEditableContentChangedOnTextArea,
        WebFeature::kWebkitEditableContentChangedOnContentEditable,
        WebFeature::kWebkitEditableContentChangedOnNotNode);
  }
}
