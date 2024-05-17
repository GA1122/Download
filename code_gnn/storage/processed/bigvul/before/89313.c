void reset_phy(void)
{
	char *name = "egiga0";
	unsigned short reg;

	if (miiphy_set_current_dev(name))
		return;

	 
	if (miiphy_read(name, CONFIG_PHY_BASE_ADR, PHY_SPEC_CTRL_REG, &reg))
		printf("Error reading PHY spec ctrl reg\n");
	if (miiphy_write(name, CONFIG_PHY_BASE_ADR, PHY_SPEC_CTRL_REG,
			 reg | PHY_RGMII_CLK_STABLE | PHY_CLSA))
		printf("Error writing PHY spec ctrl reg\n");

	 
	if (miiphy_write(name, CONFIG_PHY_BASE_ADR, PHY_LED_SEL_REG,
			 PHY_LED0_LINK | PHY_LED1_ACT | PHY_LED2_INT))
		printf("Error writing PHY LED reg\n");

	 
	miiphy_reset(name, CONFIG_PHY_BASE_ADR);
}
