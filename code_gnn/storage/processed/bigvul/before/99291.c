void ResourceMessageFilter::OnDuplicateSection(
    base::SharedMemoryHandle renderer_handle,
    base::SharedMemoryHandle* browser_handle) {
  base::SharedMemory shared_buf(renderer_handle, true, handle());
  shared_buf.GiveToProcess(base::GetCurrentProcessHandle(), browser_handle);
}
