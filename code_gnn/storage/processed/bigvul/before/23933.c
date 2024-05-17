static void MoveWindow(miccntx *context, u32 micSeq)
{
	u32 shift;

	if (micSeq > context->window) {
		shift = (micSeq - context->window) >> 1;
    
		if (shift < 32)
			context->rx >>= shift;
		else
			context->rx = 0;

		context->window = micSeq;       
	}
}
