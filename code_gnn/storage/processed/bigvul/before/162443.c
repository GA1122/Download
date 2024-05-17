    Read(gfx::mojom::GpuMemoryBufferHandleDataView data,
         gfx::GpuMemoryBufferHandle* out) {
  if (!data.ReadType(&out->type) || !data.ReadId(&out->id))
    return false;

  if (out->type == gfx::SHARED_MEMORY_BUFFER ||
      out->type == gfx::DXGI_SHARED_HANDLE ||
      out->type == gfx::ANDROID_HARDWARE_BUFFER) {
    mojo::ScopedSharedBufferHandle handle = data.TakeSharedMemoryHandle();
    if (handle.is_valid()) {
      MojoResult unwrap_result = mojo::UnwrapSharedMemoryHandle(
          std::move(handle), &out->handle, nullptr, nullptr);
      if (unwrap_result != MOJO_RESULT_OK)
        return false;
    }

    out->offset = data.offset();
    out->stride = data.stride();
  }
#if defined(OS_LINUX)
  if (out->type == gfx::NATIVE_PIXMAP &&
      !data.ReadNativePixmapHandle(&out->native_pixmap_handle))
    return false;
#endif
#if defined(OS_MACOSX) && !defined(OS_IOS)
  if (out->type == gfx::IO_SURFACE_BUFFER) {
    mach_port_t mach_port;
    MojoResult unwrap_result =
        mojo::UnwrapMachPort(data.TakeMachPort(), &mach_port);
    if (unwrap_result != MOJO_RESULT_OK)
      return false;
    out->mach_port.reset(mach_port);
  }
#endif
  return true;
}
