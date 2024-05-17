same_prefix (guint32 address1, guint32 address2, int plen)
{
	guint32 masked1 = ntohl (address1) >> (32 - plen);
	guint32 masked2 = ntohl (address2) >> (32 - plen);

	return masked1 == masked2;
}
