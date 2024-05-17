void DXVAVideoDecodeAccelerator::Flush() {
  DCHECK(CalledOnValidThread());

  DVLOG(1) << "DXVAVideoDecodeAccelerator::Flush";

  RETURN_AND_NOTIFY_ON_FAILURE((state_ == kNormal || state_ == kStopped),
      "Unexpected decoder state: " << state_, ILLEGAL_STATE,);

  state_ = kEosDrain;

  RETURN_AND_NOTIFY_ON_FAILURE(SendMFTMessage(MFT_MESSAGE_COMMAND_DRAIN, 0),
      "Failed to send drain message", PLATFORM_FAILURE,);

  while (state_ != kStopped) {
    DoDecode();
  }

  MessageLoop::current()->PostTask(FROM_HERE, base::Bind(
      &DXVAVideoDecodeAccelerator::NotifyFlushDone, this));

  state_ = kNormal;
}
