static inline void lrw_round(struct sinfo *s, void *dst, const void *src)
{
	be128_xor(dst, &s->t, src);		 
	s->fn(s->tfm, dst, dst);		 
	be128_xor(dst, dst, &s->t);		 
}
