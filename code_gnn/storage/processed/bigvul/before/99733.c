void VP9Decoder::RefreshReferenceFrames(const scoped_refptr<VP9Picture>& pic) {
  for (size_t i = 0; i < media::kVp9NumRefFrames; ++i) {
    DCHECK_IMPLIES(pic->frame_hdr->IsKeyframe(),
                   pic->frame_hdr->RefreshFlag(i));
    if (pic->frame_hdr->RefreshFlag(i))
      ref_frames_[i] = pic;
  }
}
