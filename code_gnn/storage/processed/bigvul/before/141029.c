void Document::DisableEval(const String& error_message) {
  if (!GetFrame())
    return;

  GetFrame()->GetScriptController().DisableEval(error_message);
}
