void hsb2rgb(const float hue, const float saturation, const float vlue, int &outR, int &outG, int &outB, const double maxValue )
{
	double      hh, p, q, t, ff;
	long        i;

	if(saturation <= 0.0) {
		outR = int(vlue*maxValue);
		outG = int(vlue*maxValue);
		outB = int(vlue*maxValue);
	}
	hh = hue;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = vlue * (1.0 - saturation);
	q = vlue * (1.0 - (saturation * ff));
	t = vlue * (1.0 - (saturation * (1.0 - ff)));

	switch (i) {
	case 0:
		outR = int(vlue*maxValue);
		outG = int(t*maxValue);
		outB = int(p*maxValue);
		break;
	case 1:
		outR = int(q*maxValue);
		outG = int(vlue*maxValue);
		outB = int(p*maxValue);
		break;
	case 2:
		outR = int(p*maxValue);
		outG = int(vlue*maxValue);
		outB = int(t*maxValue);
		break;

	case 3:
		outR = int(p*maxValue);
		outG = int(q*maxValue);
		outB = int(vlue*maxValue);
		break;
	case 4:
		outR = int(t*maxValue);
		outG = int(p*maxValue);
		outB = int(vlue*maxValue);
		break;
	case 5:
	default:
		outR = int(vlue*maxValue);
		outG = int(p*maxValue);
		outB = int(q*maxValue);
		break;
	}
}