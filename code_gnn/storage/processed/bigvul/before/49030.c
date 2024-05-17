static void brcmf_cfg80211_reg_notifier(struct wiphy *wiphy,
					struct regulatory_request *req)
{
	struct brcmf_cfg80211_info *cfg = wiphy_priv(wiphy);
	struct brcmf_if *ifp = netdev_priv(cfg_to_ndev(cfg));
	struct brcmf_fil_country_le ccreq;
	s32 err;
	int i;

	 
	for (i = 0; i < sizeof(req->alpha2); i++)
		if (req->alpha2[i] < 'A' || req->alpha2[i] > 'Z') {
			brcmf_err("not a ISO3166 code (0x%02x 0x%02x)\n",
				  req->alpha2[0], req->alpha2[1]);
			return;
		}

	brcmf_dbg(TRACE, "Enter: initiator=%d, alpha=%c%c\n", req->initiator,
		  req->alpha2[0], req->alpha2[1]);

	err = brcmf_fil_iovar_data_get(ifp, "country", &ccreq, sizeof(ccreq));
	if (err) {
		brcmf_err("Country code iovar returned err = %d\n", err);
		return;
	}

	err = brcmf_translate_country_code(ifp->drvr, req->alpha2, &ccreq);
	if (err)
		return;

	err = brcmf_fil_iovar_data_set(ifp, "country", &ccreq, sizeof(ccreq));
	if (err) {
		brcmf_err("Firmware rejected country setting\n");
		return;
	}
	brcmf_setup_wiphybands(wiphy);
}