void H264DPB::GetNotOutputtedPicsAppending(H264Picture::PtrVector& out) {
  for (size_t i = 0; i < pics_.size(); ++i) {
    H264Picture* pic = pics_[i];
    if (!pic->outputted)
      out.push_back(pic);
  }
}
