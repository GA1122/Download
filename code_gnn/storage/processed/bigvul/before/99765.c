bool Vp9Parser::ReadBitDepthColorSpaceSampling(Vp9FrameHeader* fhdr) {
  if (fhdr->profile == 2 || fhdr->profile == 3) {
    fhdr->bit_depth = reader_.ReadBool() ? 12 : 10;
  } else {
    fhdr->bit_depth = 8;
  }

  fhdr->color_space = static_cast<Vp9ColorSpace>(reader_.ReadLiteral(3));
  if (fhdr->color_space != Vp9ColorSpace::SRGB) {
    fhdr->yuv_range = reader_.ReadBool();
    if (fhdr->profile == 1 || fhdr->profile == 3) {
      fhdr->subsampling_x = reader_.ReadBool() ? 1 : 0;
      fhdr->subsampling_y = reader_.ReadBool() ? 1 : 0;
      if (fhdr->subsampling_x == 1 && fhdr->subsampling_y == 1) {
        DVLOG(1) << "4:2:0 color not supported in profile 1 or 3";
        return false;
      }
      bool reserved = reader_.ReadBool();
      if (reserved) {
        DVLOG(1) << "reserved bit set";
        return false;
      }
    } else {
      fhdr->subsampling_x = fhdr->subsampling_y = 1;
    }
  } else {
    if (fhdr->profile == 1 || fhdr->profile == 3) {
      fhdr->subsampling_x = fhdr->subsampling_y = 0;

      bool reserved = reader_.ReadBool();
      if (reserved) {
        DVLOG(1) << "reserved bit set";
        return false;
      }
    } else {
      DVLOG(1) << "4:4:4 color not supported in profile 0 or 2";
      return false;
    }
  }

  return true;
}
