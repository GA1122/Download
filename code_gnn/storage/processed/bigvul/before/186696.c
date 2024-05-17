    void Wait() {
    message_loop_runner_->Run();
    message_loop_runner_ = new MessageLoopRunner;
//     run_loop_->Run();
//     run_loop_ = std::make_unique<base::RunLoop>();
    }