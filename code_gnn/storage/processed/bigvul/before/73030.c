static int gdAlphaOverlayColor (int src, int dst, int max )
{
	dst = dst << 1;
	if( dst > max ) {
		 
		return dst + (src << 1) - (dst * src / max) - max;
	} else {
		 
		return dst * src / max;
	}
}
