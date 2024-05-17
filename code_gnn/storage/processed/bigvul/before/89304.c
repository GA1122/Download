int get_sda(void)
{
	I2C_TRISTATE;
	return I2C_READ;
}
