void FileManagerBrowserTestBase::RunTestMessageLoop() {
  FileManagerTestMessageListener listener;

  while (true) {
    auto message = listener.GetNextMessage();

    if (message.type == extensions::NOTIFICATION_EXTENSION_TEST_PASSED)
      return;   
    if (message.type == extensions::NOTIFICATION_EXTENSION_TEST_FAILED) {
      ADD_FAILURE() << message.message;
      return;   
    }

    const auto json = base::JSONReader::ReadDeprecated(message.message);
    const base::DictionaryValue* dictionary = nullptr;
    std::string command;
    if (!json || !json->GetAsDictionary(&dictionary) ||
        !dictionary->GetString("name", &command)) {
      message.function->Reply(std::string());
      continue;
    }

    std::string result;
    OnCommand(command, *dictionary, &result);
    if (!HasFatalFailure()) {
      message.function->Reply(result);
      continue;
    }

    LOG(INFO) << "[FAILED] " << GetTestCaseName();
    return;
  }
}
