H264Picture* H264DPB::GetLowestFrameNumWrapShortRefPic() {
  H264Picture* ret = NULL;
  for (size_t i = 0; i < pics_.size(); ++i) {
    H264Picture* pic = pics_[i];
    if (pic->ref && !pic->long_term &&
        (!ret || pic->frame_num_wrap < ret->frame_num_wrap))
      ret = pic;
  }
  return ret;
}
