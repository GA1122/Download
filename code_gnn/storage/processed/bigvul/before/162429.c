MojoResult Core::UnwrapPlatformHandle(MojoHandle mojo_handle,
                                      MojoPlatformHandle* platform_handle) {
  ScopedPlatformHandle handle;
  MojoResult result = PassWrappedPlatformHandle(mojo_handle, &handle);
  if (result != MOJO_RESULT_OK)
    return result;

  return ScopedPlatformHandleToMojoPlatformHandle(std::move(handle),
                                                  platform_handle);
}
