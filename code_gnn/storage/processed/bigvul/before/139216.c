void RenderProcessHostImpl::SendDisableAecDumpToRenderer() {
  Send(new AecDumpMsg_DisableAecDump());
}
