void WebMediaPlayerImpl::UpdateSecondaryProperties() {
  watch_time_reporter_->UpdateSecondaryProperties(
      mojom::SecondaryPlaybackProperties::New(
          pipeline_metadata_.audio_decoder_config.codec(),
          pipeline_metadata_.video_decoder_config.codec(), audio_decoder_name_,
          video_decoder_name_,
          DetermineEncryptionMode(
              pipeline_metadata_.audio_decoder_config.encryption_scheme()),
          DetermineEncryptionMode(
              pipeline_metadata_.video_decoder_config.encryption_scheme()),
          pipeline_metadata_.natural_size));
}
