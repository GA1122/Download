void GfxDeviceRGBColorSpace::getGray(GfxColor *color, GfxGray *gray) {
  *gray = clip01((GfxColorComp)(0.3 * color->c[0] +
		 0.59 * color->c[1] +
		 0.11 * color->c[2] + 0.5));
}