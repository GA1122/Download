EAS_INLINE void *PtrOfs (void *p, EAS_I32 offset)
{
 return (void*) (((EAS_U8*) p) + offset);
}
