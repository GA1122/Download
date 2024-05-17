status_t GraphicBuffer::unlockAsync(int *fenceFd)
{
 status_t res = getBufferMapper().unlockAsync(handle, fenceFd);
 return res;
}
