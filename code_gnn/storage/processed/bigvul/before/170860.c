status_t GraphicBuffer::lockAsyncYCbCr(uint32_t usage, const Rect& rect, android_ycbcr *ycbcr, int fenceFd)
{
 if (rect.left < 0 || rect.right  > this->width ||
        rect.top  < 0 || rect.bottom > this->height) {
        ALOGE("locking pixels (%d,%d,%d,%d) outside of buffer (w=%d, h=%d)",
                rect.left, rect.top, rect.right, rect.bottom,
 this->width, this->height);
 return BAD_VALUE;
 }
 status_t res = getBufferMapper().lockAsyncYCbCr(handle, usage, rect, ycbcr, fenceFd);
 return res;
}
