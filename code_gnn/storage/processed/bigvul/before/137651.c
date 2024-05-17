RenderThreadImpl::RenderThreadImpl()
    : ChildThread(Options(ShouldUseMojoChannel())) {
  Init();
}
