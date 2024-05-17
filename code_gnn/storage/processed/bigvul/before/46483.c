BN_ULONG bn_add_words (BN_ULONG *rp, const BN_ULONG *ap, const BN_ULONG *bp,int n)
{ BN_ULONG ret;
  size_t i=0;

	if (n <= 0) return 0;

	asm volatile (
	"	subq	%0,%0		\n"	 
	"	jmp	1f		\n"
	".p2align 4			\n"
	"1:	movq	(%4,%2,8),%0	\n"
	"	adcq	(%5,%2,8),%0	\n"
	"	movq	%0,(%3,%2,8)	\n"
	"	lea	1(%2),%2	\n"
	"	loop	1b		\n"
	"	sbbq	%0,%0		\n"
		: "=&r"(ret),"+c"(n),"+r"(i)
		: "r"(rp),"r"(ap),"r"(bp)
		: "cc", "memory"
	);

  return ret&1;
}
