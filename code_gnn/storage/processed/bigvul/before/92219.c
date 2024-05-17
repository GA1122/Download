static int serdes_set_mode(struct phy *phy, enum phy_mode mode)
{
	struct serdes_macro *macro = phy_get_drvdata(phy);
	unsigned int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(ocelot_serdes_muxes); i++) {
		if (macro->idx != ocelot_serdes_muxes[i].idx ||
		    mode != ocelot_serdes_muxes[i].mode)
			continue;

		if (mode != PHY_MODE_QSGMII &&
		    macro->port != ocelot_serdes_muxes[i].port)
			continue;

		ret = regmap_update_bits(macro->ctrl->regs, HSIO_HW_CFG,
					 ocelot_serdes_muxes[i].mask,
					 ocelot_serdes_muxes[i].mux);
		if (ret)
			return ret;

		if (macro->idx <= SERDES1G_MAX)
			return serdes_init_s1g(macro->ctrl->regs, macro->idx);

		 
		return -EOPNOTSUPP;
	}

	return -EINVAL;
}