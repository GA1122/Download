ScopedHandle WrapMachPort(mach_port_t port) {
  kern_return_t kr =
      mach_port_mod_refs(mach_task_self(), port, MACH_PORT_RIGHT_SEND, 1);
  MACH_LOG_IF(ERROR, kr != KERN_SUCCESS, kr)
      << "MachPortAttachmentMac mach_port_mod_refs";
  if (kr != KERN_SUCCESS)
    return ScopedHandle();

  MojoPlatformHandle platform_handle;
  platform_handle.struct_size = sizeof(MojoPlatformHandle);
  platform_handle.type = MOJO_PLATFORM_HANDLE_TYPE_MACH_PORT;
  platform_handle.value = static_cast<uint64_t>(port);

  MojoHandle mojo_handle;
  MojoResult result = MojoWrapPlatformHandle(&platform_handle, &mojo_handle);
  CHECK_EQ(result, MOJO_RESULT_OK);

  return ScopedHandle(Handle(mojo_handle));
}
