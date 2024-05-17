void DebuggerSendCommandFunction::SendResponseBody(
    base::DictionaryValue* response) {
  base::Value* error_body;
  if (response->Get("error", &error_body)) {
    base::JSONWriter::Write(*error_body, &error_);
    SendResponse(false);
    return;
  }

  base::DictionaryValue* result_body;
  SendCommand::Results::Result result;
  if (response->GetDictionary("result", &result_body))
    result.additional_properties.Swap(result_body);

  results_ = SendCommand::Results::Create(result);
  SendResponse(true);
}
