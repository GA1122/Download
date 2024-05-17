void TransportTexture::ReleaseTextures() {
  texture_map_.clear();

  bool ret = sender_->Send(new GpuTransportTextureHostMsg_ReleaseTextures(
      host_id_));
  if (!ret) {
    LOG(ERROR) << "GpuTransportTexture_ReleaseTextures failed";
  }
}
