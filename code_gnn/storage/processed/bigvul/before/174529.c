get_option_addr(struct in_addr *a, const struct dhcp_message *dhcp,
 uint8_t option)
{
 const uint8_t *p = get_option_raw(dhcp, option);

 if (!p)
 return -1;
	memcpy(&a->s_addr, p, sizeof(a->s_addr));
 return 0;
}
