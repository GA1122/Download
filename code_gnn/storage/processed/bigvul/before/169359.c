void InputEventAckWaiter::Wait() {
  if (!event_received_) {
    base::RunLoop run_loop;
    quit_ = run_loop.QuitClosure();
    run_loop.Run();
  }
}
