void HTMLFormElement::finishParsingChildren() {
  HTMLElement::finishParsingChildren();
  document().formController().restoreControlStateIn(*this);
  m_didFinishParsingChildren = true;
}
