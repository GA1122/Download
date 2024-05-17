error::Error GLES2DecoderImpl::HandleDestroyGpuFenceCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::DestroyGpuFenceCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::DestroyGpuFenceCHROMIUM*>(
          cmd_data);
  if (!features().chromium_gpu_fence) {
    return error::kUnknownCommand;
  }
  GLuint gpu_fence_id = static_cast<GLuint>(c.gpu_fence_id);
  if (!GetGpuFenceManager()->RemoveGpuFence(gpu_fence_id))
    return error::kInvalidArguments;
  return error::kNoError;
}
