void H264DPB::RemoveByPOC(int poc) {
  for (Pictures::iterator it = pics_.begin(); it != pics_.end(); ++it) {
    if ((*it)->pic_order_cnt == poc) {
      pics_.erase(it);
      return;
    }
  }
  NOTREACHED() << "Missing POC: " << poc;
}
