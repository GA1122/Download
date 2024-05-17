RenderThreadImpl::RenderThreadImpl(
    scoped_ptr<base::MessageLoop> main_message_loop)
    : ChildThread(Options(ShouldUseMojoChannel())),
      main_message_loop_(main_message_loop.Pass()) {
  Init();
}
