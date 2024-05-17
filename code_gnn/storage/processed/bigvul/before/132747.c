void ChromotingInstance::PostChromotingMessage(const std::string& method,
                                               const pp::VarDictionary& data) {
  pp::VarDictionary message;
  message.Set(pp::Var("method"), pp::Var(method));
  message.Set(pp::Var("data"), data);
  PostMessage(message);
}
