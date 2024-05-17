void PDFiumEngine::GetPasswordAndLoad() {
  getting_password_ = true;
  DCHECK(!doc_ && FPDF_GetLastError() == FPDF_ERR_PASSWORD);
  client_->GetDocumentPassword(password_factory_.NewCallbackWithOutput(
      &PDFiumEngine::OnGetPasswordComplete));
}
