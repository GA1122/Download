    mach_port(const gfx::GpuMemoryBufferHandle& handle) {
#if defined(OS_MACOSX) && !defined(OS_IOS)
  if (handle.type != gfx::IO_SURFACE_BUFFER)
    return mojo::ScopedHandle();
  return mojo::WrapMachPort(handle.mach_port.get());
#else
  return mojo::ScopedHandle();
#endif
}
