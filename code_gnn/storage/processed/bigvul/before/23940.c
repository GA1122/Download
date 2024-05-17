static void age_mic_context(miccntx *cur, miccntx *old, u8 *key, int key_len,
			    struct crypto_cipher *tfm)
{
	 
	if (cur->valid && (memcmp(cur->key, key, key_len) == 0))
		return;

	 
	memcpy(old, cur, sizeof(*cur));

	 
	memcpy(cur->key, key, key_len);
	cur->window  = 33;  
	cur->rx      = 0;   
	cur->tx      = 0;   
	cur->valid   = 1;   

	 
	emmh32_setseed(&cur->seed, key, key_len, tfm);
}
