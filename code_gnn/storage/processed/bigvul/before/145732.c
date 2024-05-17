void Fatal(ScriptContext* context, const std::string& message) {
  std::string full_message = "(";
  if (!context->is_valid())
    full_message += "Invalid ";
  full_message += context->GetContextTypeDescription();
  full_message += " context";
  if (context->extension()) {
    full_message += " for ";
    full_message += context->extension()->id();
  }
  full_message += ") ";
  full_message += message;

  ExtensionsClient* client = ExtensionsClient::Get();
  if (client->ShouldSuppressFatalErrors()) {
    console::Error(context->GetRenderFrame(), full_message);
    client->RecordDidSuppressFatalError();
  } else {
    console::Fatal(context->GetRenderFrame(), full_message);
  }
}
