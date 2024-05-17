signed char feh_wm_get_wm_is_e(void)
{
	static signed char e = -1;

	 
	if (e == -1) {
		 
		if ((XInternAtom(disp, "ENLIGHTENMENT_COMMS", True) != None)
		    && (XInternAtom(disp, "ENLIGHTENMENT_VERSION", True) != None)) {
			D(("Enlightenment detected.\n"));
			e = 1;
		} else {
			D(("Enlightenment not detected.\n"));
			e = 0;
		}
	}
	return(e);
}
