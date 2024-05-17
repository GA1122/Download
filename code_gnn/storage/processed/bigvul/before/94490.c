static inline unsigned int rfcomm_room(struct rfcomm_dlc *dlc)
{
	 
	return dlc->mtu * (dlc->tx_credits?:1);
}
