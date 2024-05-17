XYZtoRGB24(float xyz[3], uint8 rgb[3])
{
	double	r, g, b;
					 
	r =  2.690*xyz[0] + -1.276*xyz[1] + -0.414*xyz[2];
	g = -1.022*xyz[0] +  1.978*xyz[1] +  0.044*xyz[2];
	b =  0.061*xyz[0] + -0.224*xyz[1] +  1.163*xyz[2];
					 
	 
	rgb[0] = (uint8)((r<=0.) ? 0 : (r >= 1.) ? 255 : (int)(256.*sqrt(r)));
	rgb[1] = (uint8)((g<=0.) ? 0 : (g >= 1.) ? 255 : (int)(256.*sqrt(g)));
	rgb[2] = (uint8)((b<=0.) ? 0 : (b >= 1.) ? 255 : (int)(256.*sqrt(b)));
}