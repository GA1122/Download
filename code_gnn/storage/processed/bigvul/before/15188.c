int gdImageColorResolve(gdImagePtr im, int r, int g, int b)
{
	int c;
	int ct = -1;
	int op = -1;
	long rd, gd, bd, dist;
	long mindist = 3*255*255;   

	for (c = 0; c < im->colorsTotal; c++) {
		if (im->open[c]) {
			op = c;              
			continue;            
		}
		rd = (long) (im->red  [c] - r);
		gd = (long) (im->green[c] - g);
		bd = (long) (im->blue [c] - b);
		dist = rd * rd + gd * gd + bd * bd;
		if (dist < mindist) {
			if (dist == 0) {
				return c;        
			}
			mindist = dist;
			ct = c;
		}
	}
	 
	if (op == -1) {
		op = im->colorsTotal;
		if (op == gdMaxColors) {     
			return ct;           
		}
		im->colorsTotal++;
	}
	im->red  [op] = r;
	im->green[op] = g;
	im->blue [op] = b;
	im->open [op] = 0;
	return op;                   
}
