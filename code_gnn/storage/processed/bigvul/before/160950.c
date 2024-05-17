void ChromeClientImpl::DidEndEditingOnTextField(
    HTMLInputElement& input_element) {
  if (auto* fill_client =
          AutofillClientFromFrame(input_element.GetDocument().GetFrame())) {
    fill_client->TextFieldDidEndEditing(WebInputElement(&input_element));
  }
}
