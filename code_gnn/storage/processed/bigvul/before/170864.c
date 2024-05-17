status_t GraphicBuffer::unlock()
{
 status_t res = getBufferMapper().unlock(handle);
 return res;
}
