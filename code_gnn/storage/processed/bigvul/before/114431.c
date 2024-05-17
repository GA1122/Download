bool DXVAVideoDecodeAccelerator::Initialize(media::VideoCodecProfile) {
  DCHECK(CalledOnValidThread());

  RETURN_AND_NOTIFY_ON_FAILURE(pre_sandbox_init_done_,
      "PreSandbox initialization not completed", PLATFORM_FAILURE, false);

  RETURN_AND_NOTIFY_ON_FAILURE((state_ == kUninitialized),
      "Initialize: invalid state: " << state_, ILLEGAL_STATE, false);

  HRESULT hr = MFStartup(MF_VERSION, MFSTARTUP_FULL);
  RETURN_AND_NOTIFY_ON_HR_FAILURE(hr, "MFStartup failed.", PLATFORM_FAILURE,
      false);

  RETURN_AND_NOTIFY_ON_FAILURE(InitDecoder(),
      "Failed to initialize decoder", PLATFORM_FAILURE, false);

  RETURN_AND_NOTIFY_ON_FAILURE(GetStreamsInfoAndBufferReqs(),
      "Failed to get input/output stream info.", PLATFORM_FAILURE, false);

  RETURN_AND_NOTIFY_ON_FAILURE(
      SendMFTMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0),
      "Failed to start decoder", PLATFORM_FAILURE, false);

  state_ = kNormal;
  MessageLoop::current()->PostTask(FROM_HERE,
      base::Bind(&DXVAVideoDecodeAccelerator::NotifyInitializeDone, this));
  return true;
}
