void gdImageColorDeallocate (gdImagePtr im, int color)
{
	if (im->trueColor) {
		return;
	}
	 
	im->open[color] = 1;
}
