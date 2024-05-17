static void _gd2PutHeader (gdImagePtr im, gdIOCtx * out, int cs, int fmt, int cx, int cy)
{
	int i;

	 
	for (i = 0; i < 4; i++) {
		gdPutC((unsigned char) (GD2_ID[i]), out);
	}

	 

	gdPutWord(GD2_VERS, out);
	gdPutWord(im->sx, out);
	gdPutWord(im->sy, out);
	gdPutWord(cs, out);
	gdPutWord(fmt, out);
	gdPutWord(cx, out);
	gdPutWord(cy, out);
}
