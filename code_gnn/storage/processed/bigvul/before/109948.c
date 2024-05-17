H264Picture* H264DPB::GetLongRefPicByLongTermPicNum(int pic_num) {
  for (size_t i = 0; i < pics_.size(); ++i) {
    H264Picture* pic = pics_[i];
    if (pic->ref && pic->long_term && pic->long_term_pic_num == pic_num)
      return pic;
  }

  DVLOG(1) << "Missing long term pic num: " << pic_num;
  return NULL;
}
