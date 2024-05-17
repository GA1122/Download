 int post_hotkeys_pressed(void)
{
	qrio_gpio_direction_input(SELFTEST_PORT, SELFTEST_PIN);
	return qrio_get_gpio(SELFTEST_PORT, SELFTEST_PIN);
}
