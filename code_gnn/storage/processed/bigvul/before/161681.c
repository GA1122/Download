void VaapiVideoDecodeAccelerator::ImportBufferForPicture(
    int32_t picture_buffer_id,
    const gfx::GpuMemoryBufferHandle& gpu_memory_buffer_handle) {
  VLOGF(2) << "Importing picture id: " << picture_buffer_id;
  DCHECK(task_runner_->BelongsToCurrentThread());

  if (output_mode_ != Config::OutputMode::IMPORT) {
    CloseGpuMemoryBufferHandle(gpu_memory_buffer_handle);
    VLOGF(1) << "Cannot import in non-import mode";
    NotifyError(INVALID_ARGUMENT);
    return;
  }

  VaapiPicture* picture = PictureById(picture_buffer_id);
  if (!picture) {
    CloseGpuMemoryBufferHandle(gpu_memory_buffer_handle);

    VLOGF(3) << "got picture id=" << picture_buffer_id
             << " not in use (anymore?).";
    return;
  }

  if (!picture->ImportGpuMemoryBufferHandle(output_format_,
                                            gpu_memory_buffer_handle)) {
    VLOGF(1) << "Failed to import GpuMemoryBufferHandle";
    NotifyError(PLATFORM_FAILURE);
    return;
  }

  ReusePictureBuffer(picture_buffer_id);
}
