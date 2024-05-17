MojoResult UnwrapMachPort(ScopedHandle handle, mach_port_t* port) {
  MojoPlatformHandle platform_handle;
  platform_handle.struct_size = sizeof(MojoPlatformHandle);
  MojoResult result =
      MojoUnwrapPlatformHandle(handle.release().value(), &platform_handle);
  if (result != MOJO_RESULT_OK)
    return result;

  CHECK(platform_handle.type == MOJO_PLATFORM_HANDLE_TYPE_MACH_PORT ||
        platform_handle.type == MOJO_PLATFORM_HANDLE_TYPE_INVALID);
  *port = static_cast<mach_port_t>(platform_handle.value);
  return MOJO_RESULT_OK;
}
