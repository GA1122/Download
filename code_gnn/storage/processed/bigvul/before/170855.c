status_t GraphicBuffer::lock(uint32_t usage, void** vaddr)
{
 const Rect lockBounds(width, height);
 status_t res = lock(usage, lockBounds, vaddr);
 return res;
}
