vrrp_vmac_handler(vector_t *strvec)
{
	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);
	interface_t *ifp;

	__set_bit(VRRP_VMAC_BIT, &vrrp->vmac_flags);

	if (vector_size(strvec) >= 2) {
		if (strlen(strvec_slot(strvec, 1)) >= IFNAMSIZ) {
			report_config_error(CONFIG_GENERAL_ERROR, "VMAC interface name '%s' too long - ignoring", FMT_STR_VSLOT(strvec, 1));
			return;
		}

		strcpy(vrrp->vmac_ifname, strvec_slot(strvec, 1));

		 
		if ((ifp = if_get_by_ifname(vrrp->vmac_ifname, IF_NO_CREATE)) &&
		    ifp->vmac_type != MACVLAN_MODE_PRIVATE) {
			report_config_error(CONFIG_GENERAL_ERROR, "(%s) interface %s already exists and is not a private macvlan; ignoring vmac if_name", vrrp->iname, vrrp->vmac_ifname);
			vrrp->vmac_ifname[0] = '\0';
		}
	}
}