    fds(const gfx::NativePixmapHandle& pixmap_handle) {
  std::vector<mojo::ScopedHandle> handles;
#if defined(OS_LINUX)
  for (const base::FileDescriptor& fd : pixmap_handle.fds)
    handles.emplace_back(mojo::WrapPlatformFile(fd.fd));
#endif   
  return handles;
}
