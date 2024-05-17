VP9Decoder::VP9Decoder(VP9Accelerator* accelerator)
    : state_(kNeedStreamMetadata), accelerator_(accelerator) {
  DCHECK(accelerator_);
  ref_frames_.resize(media::kVp9NumRefFrames);
}
