void TransportTexture::CreateTextures(
    int n, int width, int height, Format format, std::vector<int>* textures,
    Task* done_task) {
  output_textures_ = textures;
  DCHECK(!create_task_.get());
  create_task_.reset(done_task);

  bool ret = sender_->Send(new GpuTransportTextureHostMsg_CreateTextures(
      host_id_, n, width, height, static_cast<int>(format)));
  if (!ret) {
    LOG(ERROR) << "GpuTransportTexture_CreateTextures failed";
  }
}
