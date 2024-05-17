static int ath6kl_config_btcoex_params(struct ar6_softc *ar)
{
	int r;
	WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMD sbcb_cmd;
	WMI_SET_BTCOEX_FE_ANT_CMD sbfa_cmd;

	 
	memset(&sbcb_cmd, 0, sizeof(WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMD));
	sbcb_cmd.btcoexCoLocatedBTdev = ATH6KL_BT_DEV;

	r = wmi_set_btcoex_colocated_bt_dev_cmd(ar->arWmi, &sbcb_cmd);

	if (r) {
		AR_DEBUG_PRINTF(ATH_DEBUG_ERR,
				("Unable to set collocated BT type\n"));
		return r;
	}

	 
	memset(&sbfa_cmd, 0, sizeof(WMI_SET_BTCOEX_FE_ANT_CMD));

	sbfa_cmd.btcoexFeAntType = ATH6KL_BT_ANTENNA;

	r = wmi_set_btcoex_fe_ant_cmd(ar->arWmi, &sbfa_cmd);
	if (r) {
		AR_DEBUG_PRINTF(ATH_DEBUG_ERR,
				("Unable to set fornt end antenna configuration\n"));
		return r;
	}

	return 0;
}
