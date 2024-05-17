get_option_uint32(uint32_t *i, const struct dhcp_message *dhcp, uint8_t option)
{
 const uint8_t *p = get_option_raw(dhcp, option);
 uint32_t d;

 if (!p)
 return -1;
	memcpy(&d, p, sizeof(d));
 *i = ntohl(d);
 return 0;
}
