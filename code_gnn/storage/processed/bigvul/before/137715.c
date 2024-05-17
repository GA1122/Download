bool PrintViewManagerBase::RunInnerMessageLoop() {
  static const int kPrinterSettingsTimeout = 60000;
  base::OneShotTimer quit_timer;
  base::RunLoop run_loop;
  quit_timer.Start(FROM_HERE,
                   TimeDelta::FromMilliseconds(kPrinterSettingsTimeout),
                   run_loop.QuitWhenIdleClosure());

  inside_inner_message_loop_ = true;

  {
    base::MessageLoop::ScopedNestableTaskAllower allow(
        base::MessageLoop::current());
    run_loop.Run();
  }

  bool success = true;
  if (inside_inner_message_loop_) {
    inside_inner_message_loop_ = false;
    success = false;
  }

  return success;
}
