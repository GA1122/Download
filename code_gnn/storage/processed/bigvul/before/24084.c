static void timer_func( struct net_device *dev ) {
	struct airo_info *apriv = dev->ml_priv;

 
	readConfigRid(apriv, 0);
	disable_MAC(apriv, 0);
	switch(apriv->config.authType) {
		case AUTH_ENCRYPT:
 
			apriv->config.authType = AUTH_OPEN;
			break;
		case AUTH_SHAREDKEY:
			if (apriv->keyindex < auto_wep) {
				set_wep_tx_idx(apriv, apriv->keyindex, 0, 0);
				apriv->config.authType = AUTH_SHAREDKEY;
				apriv->keyindex++;
			} else {
			         
				apriv->keyindex = 0;
				set_wep_tx_idx(apriv, apriv->defindex, 0, 0);
				apriv->config.authType = AUTH_ENCRYPT;
			}
			break;
		default:   
			apriv->config.authType = AUTH_SHAREDKEY;
	}
	set_bit (FLAG_COMMIT, &apriv->flags);
	writeConfigRid(apriv, 0);
	enable_MAC(apriv, 0);
	up(&apriv->sem);

 
	clear_bit(JOB_AUTOWEP, &apriv->jobs);
	apriv->expires = RUN_AT(HZ*3);
}
