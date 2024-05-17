void VaapiVideoDecodeAccelerator::DecodeTask() {
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  TRACE_EVENT0("Video Decoder", "VAVDA::DecodeTask");
  base::AutoLock auto_lock(lock_);

  if (state_ != kDecoding)
    return;

  DVLOG(4) << "Decode task";

  while (GetInputBuffer_Locked()) {
    DCHECK(curr_input_buffer_.get());

    AcceleratedVideoDecoder::DecodeResult res;
    {
      base::AutoUnlock auto_unlock(lock_);
      res = decoder_->Decode();
    }

    switch (res) {
      case AcceleratedVideoDecoder::kAllocateNewSurfaces:
        DVLOG(1) << "Decoder requesting a new set of surfaces";
        message_loop_->PostTask(FROM_HERE, base::Bind(
            &VaapiVideoDecodeAccelerator::InitiateSurfaceSetChange, weak_this_,
                decoder_->GetRequiredNumOfPictures(),
                decoder_->GetPicSize()));
        return;

      case AcceleratedVideoDecoder::kRanOutOfStreamData:
        ReturnCurrInputBuffer_Locked();
        break;

      case AcceleratedVideoDecoder::kRanOutOfSurfaces:
        if (!WaitForSurfaces_Locked())
          return;

        break;

      case AcceleratedVideoDecoder::kDecodeError:
        RETURN_AND_NOTIFY_ON_FAILURE(false, "Error decoding stream",
                                     PLATFORM_FAILURE, );
        return;
    }
  }
}
