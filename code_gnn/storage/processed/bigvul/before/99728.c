VP9Decoder::DecodeResult VP9Decoder::Decode() {
  while (1) {
    if (!curr_frame_hdr_) {
      scoped_ptr<media::Vp9FrameHeader> hdr(new media::Vp9FrameHeader());
      media::Vp9Parser::Result res = parser_.ParseNextFrame(hdr.get());
      switch (res) {
        case media::Vp9Parser::kOk:
          curr_frame_hdr_.reset(hdr.release());
          break;

        case media::Vp9Parser::kEOStream:
          return kRanOutOfStreamData;

        case media::Vp9Parser::kInvalidStream:
          DVLOG(1) << "Error parsing stream";
          SetError();
          return kDecodeError;
      }
    }

    if (state_ != kDecoding) {
      if (curr_frame_hdr_->IsKeyframe()) {
        state_ = kDecoding;
      } else {
        curr_frame_hdr_.reset();
        continue;
      }
    }

    if (curr_frame_hdr_->show_existing_frame) {
      size_t frame_to_show = curr_frame_hdr_->frame_to_show;
      if (frame_to_show >= ref_frames_.size() || !ref_frames_[frame_to_show]) {
        DVLOG(1) << "Request to show an invalid frame";
        SetError();
        return kDecodeError;
      }

      if (!accelerator_->OutputPicture(ref_frames_[frame_to_show])) {
        SetError();
        return kDecodeError;
      }

      curr_frame_hdr_.reset();
      continue;
    }

    gfx::Size new_pic_size(curr_frame_hdr_->width, curr_frame_hdr_->height);
    DCHECK(!new_pic_size.IsEmpty());

    if (new_pic_size != pic_size_) {
      DVLOG(1) << "New resolution: " << new_pic_size.ToString();

      if (!curr_frame_hdr_->IsKeyframe()) {
        DVLOG(1) << "Resolution change currently supported for keyframes only";
        SetError();
        return kDecodeError;
      }

      for (auto& ref_frame : ref_frames_)
        ref_frame = nullptr;

      pic_size_ = new_pic_size;
      return kAllocateNewSurfaces;
    }

    scoped_refptr<VP9Picture> pic = accelerator_->CreateVP9Picture();
    if (!pic)
      return kRanOutOfSurfaces;

    pic->frame_hdr.reset(curr_frame_hdr_.release());

    if (!DecodeAndOutputPicture(pic)) {
      SetError();
      return kDecodeError;
    }
  }
}
