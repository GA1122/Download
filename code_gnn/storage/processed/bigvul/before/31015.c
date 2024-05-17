static inline int rtm_msgindex(int msgtype)
{
	int msgindex = msgtype - RTM_BASE;

	 
	BUG_ON(msgindex < 0 || msgindex >= RTM_NR_MSGTYPES);

	return msgindex;
}
