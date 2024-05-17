void DispatchHelper(Command* command_ptr,
                    const std::string& method,
                    Response* response) {
  CHECK(method == "GET" || method == "POST" || method == "DELETE");
  scoped_ptr<Command> command(command_ptr);

  if ((method == "GET" && !command->DoesGet()) ||
      (method == "POST" && !command->DoesPost()) ||
      (method == "DELETE" && !command->DoesDelete())) {
    ListValue* methods = new ListValue;
    if (command->DoesPost())
      methods->Append(Value::CreateStringValue("POST"));
    if (command->DoesGet()) {
      methods->Append(Value::CreateStringValue("GET"));
      methods->Append(Value::CreateStringValue("HEAD"));
    }
    if (command->DoesDelete())
      methods->Append(Value::CreateStringValue("DELETE"));
    response->SetStatus(kMethodNotAllowed);
    response->SetValue(methods);
    return;
  }

  DispatchCommand(command.get(), method, response);
}
