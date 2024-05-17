bool Vp9Parser::ReadFrameSizeFromRefs(Vp9FrameHeader* fhdr) {
  for (size_t i = 0; i < kVp9NumRefsPerFrame; i++) {
    if (reader_.ReadBool()) {
      fhdr->width = ref_slots_[i].width;
      fhdr->height = ref_slots_[i].height;

      const int kMaxDimension = 1 << 16;
      if (fhdr->width == 0 || fhdr->width > kMaxDimension ||
          fhdr->height == 0 || fhdr->height > kMaxDimension) {
        DVLOG(1) << "The size of reference frame is out of range: "
                 << ref_slots_[i].width << "," << ref_slots_[i].height;
        return false;
      }
      return true;
    }
  }

  fhdr->width = reader_.ReadLiteral(16) + 1;
  fhdr->height = reader_.ReadLiteral(16) + 1;
  return true;
}
