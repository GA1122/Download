bool VaapiVideoDecodeAccelerator::Initialize(const Config& config,
                                             Client* client) {
  DCHECK(task_runner_->BelongsToCurrentThread());

  if (config.is_encrypted()) {
    NOTREACHED() << "Encrypted streams are not supported for this VDA";
    return false;
  }

  switch (config.output_mode) {
    case Config::OutputMode::ALLOCATE:
      output_format_ = vaapi_picture_factory_->GetBufferFormatForAllocateMode();
      break;

    case Config::OutputMode::IMPORT:
      output_format_ = vaapi_picture_factory_->GetBufferFormatForImportMode();
      break;

    default:
      NOTREACHED() << "Only ALLOCATE and IMPORT OutputModes are supported";
      return false;
  }

  client_ptr_factory_.reset(new base::WeakPtrFactory<Client>(client));
  client_ = client_ptr_factory_->GetWeakPtr();

  VideoCodecProfile profile = config.profile;

  base::AutoLock auto_lock(lock_);
  DCHECK_EQ(state_, kUninitialized);
  VLOGF(2) << "Initializing VAVDA, profile: " << GetProfileName(profile);

  vaapi_wrapper_ = VaapiWrapper::CreateForVideoCodec(
      VaapiWrapper::kDecode, profile, base::Bind(&ReportToUMA, VAAPI_ERROR));

  if (!vaapi_wrapper_.get()) {
    VLOGF(1) << "Failed initializing VAAPI for profile "
             << GetProfileName(profile);
    return false;
  }

  if (profile >= H264PROFILE_MIN && profile <= H264PROFILE_MAX) {
    h264_accelerator_.reset(
        new VaapiH264Accelerator(this, vaapi_wrapper_.get()));
    decoder_.reset(new H264Decoder(h264_accelerator_.get()));
  } else if (profile >= VP8PROFILE_MIN && profile <= VP8PROFILE_MAX) {
    vp8_accelerator_.reset(new VaapiVP8Accelerator(this, vaapi_wrapper_.get()));
    decoder_.reset(new VP8Decoder(vp8_accelerator_.get()));
  } else if (profile >= VP9PROFILE_MIN && profile <= VP9PROFILE_MAX) {
    vp9_accelerator_.reset(new VaapiVP9Accelerator(this, vaapi_wrapper_.get()));
    decoder_.reset(new VP9Decoder(vp9_accelerator_.get()));
  } else {
    VLOGF(1) << "Unsupported profile " << GetProfileName(profile);
    return false;
  }
  profile_ = profile;

  CHECK(decoder_thread_.Start());
  decoder_thread_task_runner_ = decoder_thread_.task_runner();

  state_ = kIdle;
  output_mode_ = config.output_mode;
  return true;
}
