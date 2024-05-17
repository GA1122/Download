void ChromeClientImpl::TextFieldDataListChanged(HTMLInputElement& input) {
  if (auto* fill_client =
          AutofillClientFromFrame(input.GetDocument().GetFrame())) {
    fill_client->DataListOptionsChanged(WebInputElement(&input));
  }
}
