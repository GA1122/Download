static inline void *nl80211hdr_put(struct sk_buff *skb, u32 pid, u32 seq,
				   int flags, u8 cmd)
{
	 
	return genlmsg_put(skb, pid, seq, &nl80211_fam, flags, cmd);
}
