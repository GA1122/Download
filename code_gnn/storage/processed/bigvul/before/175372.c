status_t MyOpusExtractor::verifyHeader(MediaBuffer *buffer, uint8_t type) {
 switch (type) {
 case 1:
 return verifyOpusHeader(buffer);
 case 3:
 return verifyOpusComments(buffer);
 default:
 return INVALID_OPERATION;
 }
}
