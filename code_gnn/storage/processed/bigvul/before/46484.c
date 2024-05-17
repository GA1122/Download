BN_ULONG bn_div_words(BN_ULONG h, BN_ULONG l, BN_ULONG d)
{	BN_ULONG ret,waste;

	asm ("divq	%4"
		: "=a"(ret),"=d"(waste)
		: "a"(l),"d"(h),"g"(d)
		: "cc");

	return ret;
}
