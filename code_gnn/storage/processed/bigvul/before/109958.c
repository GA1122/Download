void H264DPB::StorePic(H264Picture* pic) {
  DCHECK_LT(pics_.size(), kDPBMaxSize);
  DVLOG(3) << "Adding PicNum: " << pic->pic_num << " ref: " << (int)pic->ref
           << " longterm: " << (int)pic->long_term << " to DPB";
  pics_.push_back(pic);
}
