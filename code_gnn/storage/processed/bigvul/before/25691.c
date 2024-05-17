static inline int sign_extend_imm13(int imm)
{
	return imm << 19 >> 19;
}
