 static __init int seqgen_init(void)
{
	rekey_seq_generator(NULL);
// 	get_random_bytes(random_int_secret, sizeof(random_int_secret));
  	return 0;
  }