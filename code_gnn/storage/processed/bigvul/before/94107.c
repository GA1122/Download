static u8 iwlagn_key_sta_id(struct iwl_priv *priv,
			    struct ieee80211_vif *vif,
			    struct ieee80211_sta *sta)
{
	struct iwl_vif_priv *vif_priv = (void *)vif->drv_priv;
	u8 sta_id = IWL_INVALID_STATION;

	if (sta)
		sta_id = iwl_sta_id(sta);

	 
	if (!sta && vif && vif_priv->ctx) {
		switch (vif->type) {
		case NL80211_IFTYPE_STATION:
			sta_id = vif_priv->ctx->ap_sta_id;
			break;
		default:
			 
			break;
		}
	}

	return sta_id;
}
