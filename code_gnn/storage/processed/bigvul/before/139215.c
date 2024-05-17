void RenderProcessHostImpl::SendAecDumpFileToRenderer(
    int id,
    IPC::PlatformFileForTransit file_for_transit) {
  if (file_for_transit == IPC::InvalidPlatformFileForTransit())
    return;
  Send(new AecDumpMsg_EnableAecDump(id, file_for_transit));
}
