static void gdImageAntiAliasedApply (gdImagePtr im, int px, int py)
{
	float p_dist, p_alpha;
	unsigned char opacity;

	 

	int LAC_2, LBC_2;

	int Ax_Cx = im->AAL_x1 - px;
	int Ay_Cy = im->AAL_y1 - py;

	int Bx_Cx = im->AAL_x2 - px;
	int By_Cy = im->AAL_y2 - py;

	 
	if (!gdImageBoundsSafe(im, px, py)) {
		return;
	}

	 
	LAC_2 = (Ax_Cx * Ax_Cx) + (Ay_Cy * Ay_Cy);
	LBC_2 = (Bx_Cx * Bx_Cx) + (By_Cy * By_Cy);

	if (((im->AAL_LAB_2 + LAC_2) >= LBC_2) && ((im->AAL_LAB_2 + LBC_2) >= LAC_2)) {
		 
		p_dist = fabs ((float) ((Ay_Cy * im->AAL_Bx_Ax) - (Ax_Cx * im->AAL_By_Ay)) / im->AAL_LAB);
	} else {
		 
		p_dist = -1;
	}

	if ((p_dist >= 0) && (p_dist <= (float) (im->thick))) {
		p_alpha = pow (1.0 - (p_dist / 1.5), 2);

		if (p_alpha > 0) {
			if (p_alpha >= 1) {
				opacity = 255;
			} else {
				opacity = (unsigned char) (p_alpha * 255.0);
			}
			if (!im->AA_polygon || (im->AA_opacity[py][px] < opacity)) {
				im->AA_opacity[py][px] = opacity;
			}
		}
	}
}
