base::TimeDelta GLES2DecoderImpl::GetTotalTextureUploadTime() {
  return texture_state_.total_texture_upload_time +
         async_pixel_transfer_manager_->GetTotalTextureUploadTime();
}
