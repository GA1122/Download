MojoResult Core::WrapPlatformHandle(const MojoPlatformHandle* platform_handle,
                                    MojoHandle* mojo_handle) {
  ScopedPlatformHandle handle;
  MojoResult result =
      MojoPlatformHandleToScopedPlatformHandle(platform_handle, &handle);
  if (result != MOJO_RESULT_OK)
    return result;

  return CreatePlatformHandleWrapper(std::move(handle), mojo_handle);
}
