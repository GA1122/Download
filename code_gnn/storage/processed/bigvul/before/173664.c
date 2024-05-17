bool ATSParser::Stream::isVideo() const {
 switch (mStreamType) {
 case STREAMTYPE_H264:
 case STREAMTYPE_MPEG1_VIDEO:
 case STREAMTYPE_MPEG2_VIDEO:
 case STREAMTYPE_MPEG4_VIDEO:
 return true;

 default:
 return false;
 }
}
