static int spl_fit_image_get_os(const void *fit, int noffset, uint8_t *os)
{
#if CONFIG_IS_ENABLED(FIT_IMAGE_TINY) && !defined(CONFIG_SPL_OS_BOOT)
	return -ENOTSUPP;
#else
	return fit_image_get_os(fit, noffset, os);
#endif
}
