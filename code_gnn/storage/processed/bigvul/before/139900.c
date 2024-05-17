  void RunCallbackUntilCallbackInvoked(
      bool result, UsbMidiDevice::Devices* devices) {
    factory_->callback_.Run(result, devices);
    while (!client_->complete_start_session_) {
      base::RunLoop run_loop;
      run_loop.RunUntilIdle();
    }
  }
