void QuitMessageLoop(PP_Instance instance) {
  MessageLoop::current()->QuitNow();
}
