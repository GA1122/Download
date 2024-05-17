static void wl_deinit_priv(struct brcmf_cfg80211_info *cfg)
{
	cfg->dongle_up = false;	 
	brcmf_abort_scanning(cfg);
	brcmf_deinit_priv_mem(cfg);
}
