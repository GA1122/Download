 GpuFence::GpuFence(const GpuFenceHandle& handle) : type_(handle.type) {
  switch (type_) {
    case GpuFenceHandleType::kEmpty:
      break;
    case GpuFenceHandleType::kAndroidNativeFenceSync:
#if defined(OS_POSIX)
      owned_fd_.reset(handle.native_fd.fd);
#else
      NOTREACHED();
#endif
      break;
  }
}
