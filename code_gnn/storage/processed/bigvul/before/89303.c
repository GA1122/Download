int get_scl(void)
{
	return kw_gpio_get_value(KM_KIRKWOOD_SCL_PIN) ? 1 : 0;
}
