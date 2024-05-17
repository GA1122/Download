void Editor::tidyUpHTMLStructure(Document& document) {
  document.updateStyleAndLayoutTree();
  bool needsValidStructure = hasEditableStyle(document) ||
                             (document.documentElement() &&
                              hasEditableStyle(*document.documentElement()));
  if (!needsValidStructure)
    return;
  Element* existingHead = nullptr;
  Element* existingBody = nullptr;
  Element* currentRoot = document.documentElement();
  if (currentRoot) {
    if (isHTMLHtmlElement(currentRoot))
      return;
    if (isHTMLHeadElement(currentRoot))
      existingHead = currentRoot;
    else if (isHTMLBodyElement(currentRoot))
      existingBody = currentRoot;
    else if (isHTMLFrameSetElement(currentRoot))
      existingBody = currentRoot;
  }
  document.addConsoleMessage(ConsoleMessage::create(
      JSMessageSource, WarningMessageLevel,
      "document.execCommand() doesn't work with an invalid HTML structure. It "
      "is corrected automatically."));
  UseCounter::count(document, UseCounter::ExecCommandAltersHTMLStructure);

  Element* root = HTMLHtmlElement::create(document);
  if (existingHead)
    root->appendChild(existingHead);
  Element* body = nullptr;
  if (existingBody)
    body = existingBody;
  else
    body = HTMLBodyElement::create(document);
  if (document.documentElement() && body != document.documentElement())
    body->appendChild(document.documentElement());
  root->appendChild(body);
  DCHECK(!document.documentElement());
  document.appendChild(root);

}
