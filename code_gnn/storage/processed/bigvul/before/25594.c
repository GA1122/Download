static int ffloat(struct sh_fpu_soft_struct *fregs, int n)
{
	FP_DECL_EX;

	if (FPSCR_PR)
		EMU_FLOAT_X(D, DRn);
	else
		EMU_FLOAT_X(S, FRn);

	return 0;
}
