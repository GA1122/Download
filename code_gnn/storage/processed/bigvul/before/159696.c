  FakeExternalProtocolHandlerWorker(
      const shell_integration::DefaultWebClientWorkerCallback& callback,
      const std::string& protocol,
      shell_integration::DefaultWebClientState os_state)
      : shell_integration::DefaultProtocolClientWorker(callback, protocol),
        os_state_(os_state) {}
