void reset_phy(void)
{
	unsigned int oui;
	unsigned char model, rev;

	char *name = "egiga0";

	if (miiphy_set_current_dev(name))
		return;

	 
	miiphy_reset(name, CONFIG_PHY_BASE_ADR);

	 
	if (miiphy_info(name, CONFIG_PHY_BASE_ADR, &oui, &model, &rev))
		return;

	 
	if ((oui == PHY_MARVELL_OUI) &&
	    (model == PHY_MARVELL_88E1118R_MODEL)) {
		 
		if (miiphy_write(name, CONFIG_PHY_BASE_ADR,
				 PHY_MARVELL_PAGE_REG,
				 PHY_MARVELL_88E1118R_LED_CTRL_PAGE))
			printf("Error writing PHY page reg\n");

		 
		if (miiphy_write(name, CONFIG_PHY_BASE_ADR,
				 PHY_MARVELL_88E1118R_LED_CTRL_REG,
				 PHY_MARVELL_88E1118R_LED_CTRL_RESERVED |
				 PHY_MARVELL_88E1118R_LED_CTRL_LED0_1000MB |
				 PHY_MARVELL_88E1118R_LED_CTRL_LED1_ACT |
				 PHY_MARVELL_88E1118R_LED_CTRL_LED2_LINK))
			printf("Error writing PHY LED reg\n");

		 
		if (miiphy_write(name, CONFIG_PHY_BASE_ADR,
				 PHY_MARVELL_PAGE_REG,
				 PHY_MARVELL_DEFAULT_PAGE))
			printf("Error writing PHY page reg\n");
	}
}
