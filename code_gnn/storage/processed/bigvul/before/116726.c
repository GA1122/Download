void MockRenderThread::AddFilter(IPC::ChannelProxy::MessageFilter* filter) {
  filter->OnFilterAdded(&sink());
}
