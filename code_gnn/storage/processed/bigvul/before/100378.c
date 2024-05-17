void BrowserRenderProcessHost::InitVisitedLinks() {
  VisitedLinkMaster* visitedlink_master = profile()->GetVisitedLinkMaster();
  if (!visitedlink_master) {
    return;
  }

  base::SharedMemoryHandle handle_for_process;
  bool r = visitedlink_master->ShareToProcess(GetRendererProcessHandle(),
                                              &handle_for_process);
  DCHECK(r);

  if (base::SharedMemory::IsHandleValid(handle_for_process)) {
    Send(new ViewMsg_VisitedLink_NewTable(handle_for_process));
  }
}
