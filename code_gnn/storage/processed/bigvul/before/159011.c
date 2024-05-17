void PDFiumEngine::SaveSelectedFormForPrint() {
  FORM_ForceToKillFocus(form_);
  SetInFormTextArea(false);
}
