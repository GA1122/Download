static int magicmouse_firm_touch(struct magicmouse_sc *msc)
{
	int touch = -1;
	int ii;

	 
	for (ii = 0; ii < msc->ntouches; ii++) {
		int idx = msc->tracking_ids[ii];
		if (msc->touches[idx].size < 8) {
			 
		} else if (touch >= 0) {
			touch = -1;
			break;
		} else {
			touch = idx;
		}
	}

	return touch;
}