void HTMLFormElement::FinishParsingChildren() {
  HTMLElement::FinishParsingChildren();
  GetDocument().GetFormController().RestoreControlStateIn(*this);
  did_finish_parsing_children_ = true;
}
