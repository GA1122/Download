unsigned Get32u(void * Long)
{
 return (unsigned)Get32s(Long) & 0xffffffff;
}
