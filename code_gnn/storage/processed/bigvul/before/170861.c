status_t GraphicBuffer::lockYCbCr(uint32_t usage, android_ycbcr *ycbcr)
{
 const Rect lockBounds(width, height);
 status_t res = lockYCbCr(usage, lockBounds, ycbcr);
 return res;
}
