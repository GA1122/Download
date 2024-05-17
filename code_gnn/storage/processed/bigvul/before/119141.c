bool SyncSocket::PrepareTransitDescriptor(ProcessHandle peer_process_handle,
                                          TransitDescriptor* descriptor) {
  DCHECK(descriptor);
  if (!::DuplicateHandle(GetCurrentProcess(), handle(), peer_process_handle,
                         descriptor, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
    DPLOG(ERROR) << "Cannot duplicate socket handle for peer process.";
    return false;
  }
  return true;
}
