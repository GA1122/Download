void MessageLoop::Run() {
  DCHECK_EQ(this, current());
  pump_->Run(this);
}
