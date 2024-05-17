void OutOfProcessInstance::GetDocumentPassword(
    pp::CompletionCallbackWithOutput<pp::Var> callback) {
  if (password_callback_) {
    NOTREACHED();
    return;
  }

  password_callback_.reset(
      new pp::CompletionCallbackWithOutput<pp::Var>(callback));
  pp::VarDictionary message;
  message.Set(pp::Var(kType), pp::Var(kJSGetPasswordType));
  PostMessage(message);
}
