void Editor::TidyUpHTMLStructure(Document& document) {
  document.UpdateStyleAndLayoutTree();
  bool needs_valid_structure = HasEditableStyle(document) ||
                               (document.documentElement() &&
                                HasEditableStyle(*document.documentElement()));
  if (!needs_valid_structure)
    return;
  Element* existing_head = nullptr;
  Element* existing_body = nullptr;
  Element* current_root = document.documentElement();
  if (current_root) {
    if (isHTMLHtmlElement(current_root))
      return;
    if (isHTMLHeadElement(current_root))
      existing_head = current_root;
    else if (isHTMLBodyElement(current_root))
      existing_body = current_root;
    else if (isHTMLFrameSetElement(current_root))
      existing_body = current_root;
  }
  document.AddConsoleMessage(ConsoleMessage::Create(
      kJSMessageSource, kWarningMessageLevel,
      "document.execCommand() doesn't work with an invalid HTML structure. It "
      "is corrected automatically."));
  UseCounter::Count(document, WebFeature::kExecCommandAltersHTMLStructure);

  Element* root = HTMLHtmlElement::Create(document);
  if (existing_head)
    root->AppendChild(existing_head);
  Element* body = nullptr;
  if (existing_body)
    body = existing_body;
  else
    body = HTMLBodyElement::Create(document);
  if (document.documentElement() && body != document.documentElement())
    body->AppendChild(document.documentElement());
  root->AppendChild(body);
  DCHECK(!document.documentElement());
  document.AppendChild(root);

}
