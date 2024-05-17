void OutOfProcessInstance::Print() {
  if (!engine_->HasPermission(PDFEngine::PERMISSION_PRINT_LOW_QUALITY) &&
      !engine_->HasPermission(PDFEngine::PERMISSION_PRINT_HIGH_QUALITY)) {
    return;
  }

  pp::CompletionCallback callback =
      print_callback_factory_.NewCallback(&OutOfProcessInstance::OnPrint);
  pp::Module::Get()->core()->CallOnMainThread(0, callback);
}
