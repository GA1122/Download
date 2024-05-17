vrrp_goto_master(vrrp_t * vrrp)
{
	 
	vrrp->wantstate = VRRP_STATE_MAST;
	vrrp_state_goto_master(vrrp);
}
