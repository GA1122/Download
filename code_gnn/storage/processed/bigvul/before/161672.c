void VaapiVideoDecodeAccelerator::DecodeTask() {
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  base::AutoLock auto_lock(lock_);

  if (state_ != kDecoding)
    return;

  VLOGF(4) << "Decode task";

  while (GetInputBuffer_Locked()) {
    DCHECK(curr_input_buffer_.get());

    if (curr_input_buffer_->IsFlushRequest()) {
      FlushTask();
      break;
    }

    AcceleratedVideoDecoder::DecodeResult res;
    {
      base::AutoUnlock auto_unlock(lock_);
      TRACE_EVENT0("Video Decoder", "VAVDA::Decode");
      res = decoder_->Decode();
    }

    switch (res) {
      case AcceleratedVideoDecoder::kAllocateNewSurfaces:
        VLOGF(2) << "Decoder requesting a new set of surfaces";
        task_runner_->PostTask(
            FROM_HERE,
            base::Bind(&VaapiVideoDecodeAccelerator::InitiateSurfaceSetChange,
                       weak_this_, decoder_->GetRequiredNumOfPictures(),
                       decoder_->GetPicSize()));
        return;

      case AcceleratedVideoDecoder::kRanOutOfStreamData:
        ReturnCurrInputBuffer_Locked();
        break;

      case AcceleratedVideoDecoder::kRanOutOfSurfaces:
        if (!WaitForSurfaces_Locked())
          return;

        break;

      case AcceleratedVideoDecoder::kNeedContextUpdate:
        NOTREACHED() << "Context updates not supported";
        return;

      case AcceleratedVideoDecoder::kDecodeError:
        RETURN_AND_NOTIFY_ON_FAILURE(false, "Error decoding stream",
                                     PLATFORM_FAILURE, );
        return;
    }
  }
}
