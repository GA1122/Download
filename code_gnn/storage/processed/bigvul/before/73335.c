static inline int16_t pre_blur1_func(int16_t p1, int16_t z0, int16_t n1)
{
     
    return (uint16_t)(((uint16_t)(p1 + n1) >> 1) + z0 + 1) >> 1;
}
