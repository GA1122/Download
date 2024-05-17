GpuFenceHandle GpuFence::GetGpuFenceHandle() const {
  gfx::GpuFenceHandle handle;
  switch (type_) {
    case GpuFenceHandleType::kEmpty:
      break;
    case GpuFenceHandleType::kAndroidNativeFenceSync:
#if defined(OS_POSIX)
      handle.type = gfx::GpuFenceHandleType::kAndroidNativeFenceSync;
      handle.native_fd = base::FileDescriptor(owned_fd_.get(),
                                               false);
#else
      NOTREACHED();
#endif
      break;
  }
  return handle;
}
