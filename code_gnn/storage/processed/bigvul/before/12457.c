int SRP_Verify_A_mod_N(BIGNUM *A, BIGNUM *N)
	{
	 
	return SRP_Verify_B_mod_N(A,N) ;
	}
