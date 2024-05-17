static inline bool serpent_fpu_begin(bool fpu_enabled, unsigned int nbytes)
{
	 
	return glue_fpu_begin(SERPENT_BLOCK_SIZE, 8, NULL, fpu_enabled, nbytes);
}
