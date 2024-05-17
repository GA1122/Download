const struct ldap_control_handler *samba_ldap_control_handlers(void)
{
	return ldap_known_controls;
}
