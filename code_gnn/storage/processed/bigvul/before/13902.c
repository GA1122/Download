fz_skip_space(fz_context *ctx, fz_stream *stm)
{
	do
	{
		int c = fz_peek_byte(ctx, stm);
		if (c > 32 && c != EOF)
			return;
		(void)fz_read_byte(ctx, stm);
	}
	while (1);
}
