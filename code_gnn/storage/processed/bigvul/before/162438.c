MojoResult UnwrapPlatformFile(ScopedHandle handle, base::PlatformFile* file) {
  MojoPlatformHandle platform_handle;
  platform_handle.struct_size = sizeof(MojoPlatformHandle);
  MojoResult result =
      MojoUnwrapPlatformHandle(handle.release().value(), &platform_handle);
  if (result != MOJO_RESULT_OK)
    return result;

  if (platform_handle.type == MOJO_PLATFORM_HANDLE_TYPE_INVALID) {
    *file = base::kInvalidPlatformFile;
  } else {
    CHECK_EQ(platform_handle.type, kPlatformFileHandleType);
    *file = PlatformFileFromPlatformHandleValue(platform_handle.value);
  }

  return MOJO_RESULT_OK;
}
