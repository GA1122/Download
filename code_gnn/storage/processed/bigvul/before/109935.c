void GpuVideoDecodeAccelerator::OnAssignPictureBuffers(
      const std::vector<int32>& buffer_ids,
      const std::vector<uint32>& texture_ids,
      const std::vector<gfx::Size>& sizes) {
  DCHECK(stub_ && stub_->decoder());   
  gpu::gles2::GLES2Decoder* command_decoder = stub_->decoder();
  gpu::gles2::TextureManager* texture_manager =
      command_decoder->GetContextGroup()->texture_manager();

  std::vector<media::PictureBuffer> buffers;
  for (uint32 i = 0; i < buffer_ids.size(); ++i) {
    gpu::gles2::TextureManager::TextureInfo* info =
        texture_manager->GetTextureInfo(texture_ids[i]);
    if (!info) {
      DLOG(FATAL) << "Failed to find texture id " << texture_ids[i];
      NotifyError(media::VideoDecodeAccelerator::INVALID_ARGUMENT);
      return;
    }
    if (!texture_manager->ClearRenderableLevels(command_decoder, info)) {
      DLOG(FATAL) << "Failed to Clear texture id " << texture_ids[i];
      NotifyError(media::VideoDecodeAccelerator::PLATFORM_FAILURE);
      return;
    }
    uint32 service_texture_id;
    if (!command_decoder->GetServiceTextureId(
            texture_ids[i], &service_texture_id)) {
      DLOG(FATAL) << "Failed to translate texture!";
      NotifyError(media::VideoDecodeAccelerator::PLATFORM_FAILURE);
      return;
    }
    buffers.push_back(media::PictureBuffer(
        buffer_ids[i], sizes[i], service_texture_id));
  }
  video_decode_accelerator_->AssignPictureBuffers(buffers);
}
