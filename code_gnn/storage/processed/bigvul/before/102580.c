void NaClProcessHost::OpenIrtFileDone(base::PlatformFileError error_code,
                                      base::PassPlatformFile file,
                                      bool created) {
  std::vector<nacl::FileDescriptor> handles_for_renderer;
  base::ProcessHandle nacl_process_handle;
  bool have_irt_file = false;
  if (base::PLATFORM_FILE_OK == error_code) {
    internal_->sockets_for_sel_ldr.push_back(file.ReleaseValue());
    have_irt_file = true;
  } else {
    LOG(ERROR) << "Failed to open the NaCl IRT library file";
  }

  for (size_t i = 0; i < internal_->sockets_for_renderer.size(); i++) {
#if defined(OS_WIN)
    HANDLE handle_in_renderer;
    if (!DuplicateHandle(base::GetCurrentProcessHandle(),
                         reinterpret_cast<HANDLE>(
                             internal_->sockets_for_renderer[i]),
                         chrome_render_message_filter_->peer_handle(),
                         &handle_in_renderer,
                         0,   
                         FALSE,
                         DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS)) {
      LOG(ERROR) << "DuplicateHandle() failed";
      delete this;
      return;
    }
    handles_for_renderer.push_back(
        reinterpret_cast<nacl::FileDescriptor>(handle_in_renderer));
#else
    nacl::FileDescriptor imc_handle;
    imc_handle.fd = internal_->sockets_for_renderer[i];
    imc_handle.auto_close = true;
    handles_for_renderer.push_back(imc_handle);
#endif
  }

#if defined(OS_WIN)
  if (!DuplicateHandle(base::GetCurrentProcessHandle(),
                       handle(),
                       chrome_render_message_filter_->peer_handle(),
                       &nacl_process_handle,
                       PROCESS_DUP_HANDLE,
                       FALSE,
                       0)) {
    LOG(ERROR) << "DuplicateHandle() failed";
    delete this;
    return;
  }
#else
  nacl_process_handle = handle();
#endif

  base::ProcessId nacl_process_id = base::GetProcId(handle());

  ViewHostMsg_LaunchNaCl::WriteReplyParams(
      reply_msg_, handles_for_renderer, nacl_process_handle, nacl_process_id);
  chrome_render_message_filter_->Send(reply_msg_);
  chrome_render_message_filter_ = NULL;
  reply_msg_ = NULL;
  internal_->sockets_for_renderer.clear();

  std::vector<nacl::FileDescriptor> handles_for_sel_ldr;
  for (size_t i = 0; i < internal_->sockets_for_sel_ldr.size(); i++) {
#if defined(OS_WIN)
    HANDLE channel;
    if (!DuplicateHandle(GetCurrentProcess(),
                         reinterpret_cast<HANDLE>(
                             internal_->sockets_for_sel_ldr[i]),
                         handle(),
                         &channel,
                         0,   
                         FALSE,
                         DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS)) {
      LOG(ERROR) << "DuplicateHandle() failed";
      delete this;
      return;
    }
    handles_for_sel_ldr.push_back(
        reinterpret_cast<nacl::FileDescriptor>(channel));
#else
    nacl::FileDescriptor channel;
    channel.fd = internal_->sockets_for_sel_ldr[i];
    channel.auto_close = true;
    handles_for_sel_ldr.push_back(channel);
#endif
  }

#if defined(OS_MACOSX)
  base::SharedMemory memory_buffer;
  if (!memory_buffer.CreateAnonymous(  1)) {
    LOG(ERROR) << "Failed to allocate memory buffer";
    delete this;
    return;
  }
  nacl::FileDescriptor memory_fd;
  memory_fd.fd = dup(memory_buffer.handle().fd);
  if (memory_fd.fd < 0) {
    LOG(ERROR) << "Failed to dup() a file descriptor";
    delete this;
    return;
  }
  memory_fd.auto_close = true;
  handles_for_sel_ldr.push_back(memory_fd);
#endif

  Send(new NaClProcessMsg_Start(handles_for_sel_ldr, have_irt_file));
  internal_->sockets_for_sel_ldr.clear();
}
