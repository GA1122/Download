void nl80211_send_reg_change_event(struct regulatory_request *request)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_REG_CHANGE);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	 
	NLA_PUT_U8(msg, NL80211_ATTR_REG_INITIATOR, request->initiator);

	if (request->alpha2[0] == '0' && request->alpha2[1] == '0')
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_WORLD);
	else if (request->alpha2[0] == '9' && request->alpha2[1] == '9')
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_CUSTOM_WORLD);
	else if ((request->alpha2[0] == '9' && request->alpha2[1] == '8') ||
		 request->intersect)
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_INTERSECTION);
	else {
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_COUNTRY);
		NLA_PUT_STRING(msg, NL80211_ATTR_REG_ALPHA2, request->alpha2);
	}

	if (wiphy_idx_valid(request->wiphy_idx))
		NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, request->wiphy_idx);

	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}

	rcu_read_lock();
	genlmsg_multicast_allns(msg, 0, nl80211_regulatory_mcgrp.id,
				GFP_ATOMIC);
	rcu_read_unlock();

	return;

nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}