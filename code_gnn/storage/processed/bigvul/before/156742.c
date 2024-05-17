  void WaitUntilStable() {
    uint32_t current_frame_number = view_->RendererFrameNumber();
    uint32_t previous_frame_number;

    do {
      base::RunLoop run_loop;
      base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
          FROM_HERE, run_loop.QuitClosure(), delta_);
      run_loop.Run();
      previous_frame_number = current_frame_number;
      current_frame_number = view_->RendererFrameNumber();
    } while (current_frame_number != previous_frame_number);
  }
