status_t GraphicBuffer::lockAsyncYCbCr(uint32_t usage, android_ycbcr *ycbcr, int fenceFd)
{
 const Rect lockBounds(width, height);
 status_t res = lockAsyncYCbCr(usage, lockBounds, ycbcr, fenceFd);
 return res;
}
