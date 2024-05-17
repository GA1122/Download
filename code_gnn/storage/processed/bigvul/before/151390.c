void FrameFetchContext::AddConsoleMessage(ConsoleMessage* message) const {
  if (IsDetached())
    return;

  if (document_ && document_->GetFrame())
    document_->AddConsoleMessage(message);
  else
    GetFrame()->Console().AddMessage(message);
}
