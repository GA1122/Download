void DXVAVideoDecodeAccelerator::Reset() {
  DCHECK(CalledOnValidThread());

  DVLOG(1) << "DXVAVideoDecodeAccelerator::Reset";

  RETURN_AND_NOTIFY_ON_FAILURE((state_ == kNormal || state_ == kStopped),
      "Reset: invalid state: " << state_, ILLEGAL_STATE,);

  state_ = kResetting;

  RETURN_AND_NOTIFY_ON_FAILURE(SendMFTMessage(MFT_MESSAGE_COMMAND_FLUSH, 0),
      "Reset: Failed to send message.", PLATFORM_FAILURE,);

  MessageLoop::current()->PostTask(FROM_HERE, base::Bind(
      &DXVAVideoDecodeAccelerator::NotifyResetDone, this));

  state_ = DXVAVideoDecodeAccelerator::kNormal;
}
