int post_hotkeys_pressed(void)
{
#if defined(CONFIG_KM_COGE5UN)
	return kw_gpio_get_value(KM_POST_EN_L);
#else
	return !kw_gpio_get_value(KM_POST_EN_L);
#endif
}
