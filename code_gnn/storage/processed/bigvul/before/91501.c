static int gifPutWord(int w, gdIOCtx *out)
{
	 
	gdPutC(w & 0xFF, out);
	gdPutC((w >> 8) & 0xFF, out);
	return 0;
}
