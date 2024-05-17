static inline int16_t clamp16(int32_t sample)
{
 if ((sample>>15) ^ (sample>>31))
        sample = 0x7FFF ^ (sample>>31);
 return sample;
}
