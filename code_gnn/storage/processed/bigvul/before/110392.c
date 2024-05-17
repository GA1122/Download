void RunMessageLoop(PP_Instance instance) {
  MessageLoop::ScopedNestableTaskAllower allow(MessageLoop::current());
  MessageLoop::current()->Run();
}
