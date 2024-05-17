bool VP9Decoder::DecodeAndOutputPicture(scoped_refptr<VP9Picture> pic) {
  DCHECK(!pic_size_.IsEmpty());
  DCHECK(pic->frame_hdr);

  if (!accelerator_->SubmitDecode(pic, parser_.GetSegmentation(),
                                  parser_.GetLoopFilter(), ref_frames_))
    return false;

  if (pic->frame_hdr->show_frame) {
    if (!accelerator_->OutputPicture(pic))
      return false;
  }

  RefreshReferenceFrames(pic);
  return true;
}
