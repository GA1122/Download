  void OnDeviceOpened(bool success,
                      const std::string& label,
                      const MediaStreamDevice& device) {
    base::Closure quit_closure = quit_closures_.front();
    quit_closures_.pop();
    task_runner_->PostTask(FROM_HERE, base::ResetAndReturn(&quit_closure));
    if (success) {
      label_ = label;
      opened_device_ = device;
      OnDeviceOpenSuccess();
    }
  }
