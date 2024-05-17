ScopedTextureUploadTimer::~ScopedTextureUploadTimer() {
  decoder_->texture_upload_count_++;
  decoder_->total_texture_upload_time_ +=
      base::TimeTicks::HighResNow() - begin_time_;
}
