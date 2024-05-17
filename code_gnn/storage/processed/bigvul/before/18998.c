static void syn_flood_warning(const struct sk_buff *skb)
{
	const char *msg;

#ifdef CONFIG_SYN_COOKIES
	if (sysctl_tcp_syncookies)
		msg = "Sending cookies";
	else
#endif
		msg = "Dropping request";

	pr_info("TCP: Possible SYN flooding on port %d. %s.\n",
				ntohs(tcp_hdr(skb)->dest), msg);
}