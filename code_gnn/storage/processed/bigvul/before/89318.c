void set_sda(int state)
{
	I2C_ACTIVE;
	I2C_SDA(state);
}
