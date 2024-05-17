void BrowserEventRouter::CommandExecuted(Profile* profile,
                                         const std::string& extension_id,
                                         const std::string& command) {
  scoped_ptr<ListValue> args(new ListValue());
  args->Append(Value::CreateStringValue(command));

  DispatchEventToExtension(profile,
                           extension_id,
                           "commands.onCommand",
                           args.Pass(),
                           EventRouter::USER_GESTURE_ENABLED);
}
