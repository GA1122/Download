void BrowserRenderProcessHost::SendUserScriptsUpdate(
    base::SharedMemory *shared_memory) {
  base::SharedMemoryHandle handle_for_process;
  if (!shared_memory->ShareToProcess(GetRendererProcessHandle(),
                                     &handle_for_process)) {
    return;
  }

  if (base::SharedMemory::IsHandleValid(handle_for_process)) {
    Send(new ViewMsg_UserScripts_UpdatedScripts(handle_for_process));
  }
}
