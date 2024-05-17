static void RunAutofocusTask(ExecutionContext* context) {
  if (!context)
    return;

  Document* document = ToDocument(context);
  if (Element* element = document->AutofocusElement()) {
    document->SetAutofocusElement(0);
    element->focus();
  }
}
