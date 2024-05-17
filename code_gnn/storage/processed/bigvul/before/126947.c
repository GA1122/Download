void AudioInputRendererHost::DoCompleteCreation(
    media::AudioInputController* controller) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  if (!peer_handle()) {
    NOTREACHED() << "Renderer process handle is invalid.";
    DeleteEntryOnError(entry);
    return;
  }

  if (!entry->controller->LowLatencyMode()) {
    NOTREACHED() << "Only low-latency mode is supported.";
    DeleteEntryOnError(entry);
    return;
  }

  base::SharedMemoryHandle foreign_memory_handle;
  if (!entry->shared_memory.ShareToProcess(peer_handle(),
                                           &foreign_memory_handle)) {
    DeleteEntryOnError(entry);
    return;
  }

  AudioInputSyncWriter* writer =
      static_cast<AudioInputSyncWriter*>(entry->writer.get());

#if defined(OS_WIN)
  base::SyncSocket::Handle foreign_socket_handle;
#else
  base::FileDescriptor foreign_socket_handle;
#endif

  if (!writer->PrepareForeignSocketHandle(peer_handle(),
                                          &foreign_socket_handle)) {
    DeleteEntryOnError(entry);
    return;
  }

  Send(new AudioInputMsg_NotifyStreamCreated(entry->stream_id,
      foreign_memory_handle, foreign_socket_handle,
      entry->shared_memory.created_size()));
}
