HWB_Diff (int r1, int g1, int b1, int r2, int g2, int b2)
{
	RGBType RGB1, RGB2;
	HWBType HWB1, HWB2;
	float diff;

	SETUP_RGB (RGB1, r1, g1, b1);
	SETUP_RGB (RGB2, r2, g2, b2);

	RGB_to_HWB (RGB1, &HWB1);
	RGB_to_HWB (RGB2, &HWB2);

	 

	if ((HWB1.H == HWB_UNDEFINED) || (HWB2.H == HWB_UNDEFINED)) {
		diff = 0;			 
	} else {
		diff = fabs (HWB1.H - HWB2.H);
		if (diff > 3) {
			diff = 6 - diff;	 
		}
	}

	diff =
	    diff * diff + (HWB1.W - HWB2.W) * (HWB1.W - HWB2.W) + (HWB1.B -
	            HWB2.B) * (HWB1.B -
	                       HWB2.B);

	return diff;
}
