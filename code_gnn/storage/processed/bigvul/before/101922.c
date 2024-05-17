void PrintingMessageFilter::OnDuplicateSection(
    base::SharedMemoryHandle renderer_handle,
    base::SharedMemoryHandle* browser_handle) {
  base::SharedMemory shared_buf(renderer_handle, true, peer_handle());
  shared_buf.GiveToProcess(base::GetCurrentProcessHandle(), browser_handle);
}
