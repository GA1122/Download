H264Picture* H264DPB::GetShortRefPicByPicNum(int pic_num) {
  for (size_t i = 0; i < pics_.size(); ++i) {
    H264Picture* pic = pics_[i];
    if (pic->ref && !pic->long_term && pic->pic_num == pic_num)
      return pic;
  }

  DVLOG(1) << "Missing short ref pic num: " << pic_num;
  return NULL;
}
