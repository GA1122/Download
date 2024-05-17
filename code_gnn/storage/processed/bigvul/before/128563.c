void Instance::GetDocumentPassword(
    pp::CompletionCallbackWithOutput<pp::Var> callback) {
  std::string message(GetLocalizedString(PP_RESOURCESTRING_PDFGETPASSWORD));
  pp::Var result = pp::PDF::ModalPromptForPassword(this, message);
  *callback.output() = result.pp_var();
  callback.Run(PP_OK);
}
