static int comparewithmap(gdImagePtr im1, gdImagePtr im2, int c1, int c2, int *colorMap)
{
	if(!colorMap) {
		return c1 == c2;
	}

	if(-2 != colorMap[c1]) {
		return colorMap[c1] == c2;
	}

	return (colorMap[c1] = gdImageColorExactAlpha(im2, im1->red[c1], im1->green[c1], im1->blue[c1], im1->alpha[c1])) == c2;
}
