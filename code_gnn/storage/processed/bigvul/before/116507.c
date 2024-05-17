void SendCommandDebuggerFunction::SendResponseBody(
    DictionaryValue* dictionary) {
  Value* error_body;
  if (dictionary->Get("error", &error_body)) {
    base::JSONWriter::Write(error_body, false, &error_);
    SendResponse(false);
    return;
  }

  Value* result_body;
  if (dictionary->Get("result", &result_body))
    result_.reset(result_body->DeepCopy());
  else
    result_.reset(new DictionaryValue());
  SendResponse(true);
}
