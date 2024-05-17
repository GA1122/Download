void VaapiWrapper::VADisplayState::SetDrmFd(base::PlatformFile fd) {
  drm_fd_.reset(HANDLE_EINTR(dup(fd)));
}
