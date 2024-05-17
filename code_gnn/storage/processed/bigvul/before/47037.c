static int __init sha1_ssse3_mod_init(void)
{
	char *algo_name;

	 
	if (cpu_has_ssse3) {
		sha1_transform_asm = sha1_transform_ssse3;
		algo_name = "SSSE3";
	}

#ifdef CONFIG_AS_AVX
	 
	if (avx_usable()) {
		sha1_transform_asm = sha1_transform_avx;
		algo_name = "AVX";
#ifdef CONFIG_AS_AVX2
		 
		if (avx2_usable()) {
			sha1_transform_asm = sha1_apply_transform_avx2;
			algo_name = "AVX2";
		}
#endif
	}
#endif

	if (sha1_transform_asm) {
		pr_info("Using %s optimized SHA-1 implementation\n", algo_name);
		return crypto_register_shash(&alg);
	}
	pr_info("Neither AVX nor AVX2 nor SSSE3 is available/usable.\n");

	return -ENODEV;
}
