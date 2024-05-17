void HTMLInputElement::DidMoveToNewDocument(Document& old_document) {
  if (ImageLoader())
    ImageLoader()->ElementDidMoveToNewDocument();

  if (type() == InputTypeNames::radio)
    GetTreeScope().GetRadioButtonGroupScope().RemoveButton(this);

  TextControlElement::DidMoveToNewDocument(old_document);
}
