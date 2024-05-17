void ChromeClientImpl::DidChangeValueInTextField(
    HTMLFormControlElement& element) {
  Document& doc = element.GetDocument();
  if (auto* fill_client = AutofillClientFromFrame(doc.GetFrame()))
    fill_client->TextFieldDidChange(WebFormControlElement(&element));

  UseCounter::Count(doc, doc.IsSecureContext()
                             ? WebFeature::kFieldEditInSecureContext
                             : WebFeature::kFieldEditInNonSecureContext);
  doc.MaybeQueueSendDidEditFieldInInsecureContext();
  web_view_->PageImportanceSignals()->SetHadFormInteraction();
}
