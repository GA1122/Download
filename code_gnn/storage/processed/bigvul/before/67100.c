f2flac16_array (const float *src, int32_t *dest, int count, int normalize)
{	float normfact = normalize ? (1.0 * 0x7FFF) : 1.0 ;

	while (--count >= 0)
		dest [count] = lrintf (src [count] * normfact) ;
}  