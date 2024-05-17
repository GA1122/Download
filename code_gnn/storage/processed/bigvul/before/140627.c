void OutOfProcessInstance::IsSelectingChanged(bool is_selecting) {
  pp::VarDictionary message;
  message.Set(kType, kJSSetIsSelectingType);
  message.Set(kJSIsSelecting, pp::Var(is_selecting));
  PostMessage(message);
}
