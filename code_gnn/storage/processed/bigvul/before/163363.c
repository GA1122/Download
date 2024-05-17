void RenderThreadImpl::OnChannelError() {
  CHECK(!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSingleProcess));
  ChildThreadImpl::OnChannelError();
}
