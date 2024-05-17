static s32 atl2_phy_setup_autoneg_adv(struct atl2_hw *hw)
{
	s32 ret_val;
	s16 mii_autoneg_adv_reg;

	 
	mii_autoneg_adv_reg = MII_AR_DEFAULT_CAP_MASK;

	 

	 
	mii_autoneg_adv_reg &= ~MII_AR_SPEED_MASK;

	 
	switch (hw->MediaType) {
	case MEDIA_TYPE_AUTO_SENSOR:
		mii_autoneg_adv_reg |=
			(MII_AR_10T_HD_CAPS |
			MII_AR_10T_FD_CAPS  |
			MII_AR_100TX_HD_CAPS|
			MII_AR_100TX_FD_CAPS);
		hw->autoneg_advertised =
			ADVERTISE_10_HALF |
			ADVERTISE_10_FULL |
			ADVERTISE_100_HALF|
			ADVERTISE_100_FULL;
		break;
	case MEDIA_TYPE_100M_FULL:
		mii_autoneg_adv_reg |= MII_AR_100TX_FD_CAPS;
		hw->autoneg_advertised = ADVERTISE_100_FULL;
		break;
	case MEDIA_TYPE_100M_HALF:
		mii_autoneg_adv_reg |= MII_AR_100TX_HD_CAPS;
		hw->autoneg_advertised = ADVERTISE_100_HALF;
		break;
	case MEDIA_TYPE_10M_FULL:
		mii_autoneg_adv_reg |= MII_AR_10T_FD_CAPS;
		hw->autoneg_advertised = ADVERTISE_10_FULL;
		break;
	default:
		mii_autoneg_adv_reg |= MII_AR_10T_HD_CAPS;
		hw->autoneg_advertised = ADVERTISE_10_HALF;
		break;
	}

	 
	mii_autoneg_adv_reg |= (MII_AR_ASM_DIR | MII_AR_PAUSE);

	hw->mii_autoneg_adv_reg = mii_autoneg_adv_reg;

	ret_val = atl2_write_phy_reg(hw, MII_ADVERTISE, mii_autoneg_adv_reg);

	if (ret_val)
		return ret_val;

	return 0;
}
