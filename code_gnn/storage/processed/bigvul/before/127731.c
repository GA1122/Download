void TestTransactionConsumer::DidFinish(int result) {
  state_ = DONE;
  error_ = result;
  if (--quit_counter_ == 0)
    base::MessageLoop::current()->QuitWhenIdle();
}
