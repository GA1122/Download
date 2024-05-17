static int nl80211_set_wowlan(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct nlattr *tb[NUM_NL80211_WOWLAN_TRIG];
	struct cfg80211_wowlan no_triggers = {};
	struct cfg80211_wowlan new_triggers = {};
	struct wiphy_wowlan_support *wowlan = &rdev->wiphy.wowlan;
	int err, i;

	if (!rdev->wiphy.wowlan.flags && !rdev->wiphy.wowlan.n_patterns)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_WOWLAN_TRIGGERS])
		goto no_triggers;

	err = nla_parse(tb, MAX_NL80211_WOWLAN_TRIG,
			nla_data(info->attrs[NL80211_ATTR_WOWLAN_TRIGGERS]),
			nla_len(info->attrs[NL80211_ATTR_WOWLAN_TRIGGERS]),
			nl80211_wowlan_policy);
	if (err)
		return err;

	if (tb[NL80211_WOWLAN_TRIG_ANY]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_ANY))
			return -EINVAL;
		new_triggers.any = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_DISCONNECT]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_DISCONNECT))
			return -EINVAL;
		new_triggers.disconnect = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_MAGIC_PKT]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_MAGIC_PKT))
			return -EINVAL;
		new_triggers.magic_pkt = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_PKT_PATTERN]) {
		struct nlattr *pat;
		int n_patterns = 0;
		int rem, pat_len, mask_len;
		struct nlattr *pat_tb[NUM_NL80211_WOWLAN_PKTPAT];

		nla_for_each_nested(pat, tb[NL80211_WOWLAN_TRIG_PKT_PATTERN],
				    rem)
			n_patterns++;
		if (n_patterns > wowlan->n_patterns)
			return -EINVAL;

		new_triggers.patterns = kcalloc(n_patterns,
						sizeof(new_triggers.patterns[0]),
						GFP_KERNEL);
		if (!new_triggers.patterns)
			return -ENOMEM;

		new_triggers.n_patterns = n_patterns;
		i = 0;

		nla_for_each_nested(pat, tb[NL80211_WOWLAN_TRIG_PKT_PATTERN],
				    rem) {
			nla_parse(pat_tb, MAX_NL80211_WOWLAN_PKTPAT,
				  nla_data(pat), nla_len(pat), NULL);
			err = -EINVAL;
			if (!pat_tb[NL80211_WOWLAN_PKTPAT_MASK] ||
			    !pat_tb[NL80211_WOWLAN_PKTPAT_PATTERN])
				goto error;
			pat_len = nla_len(pat_tb[NL80211_WOWLAN_PKTPAT_PATTERN]);
			mask_len = DIV_ROUND_UP(pat_len, 8);
			if (nla_len(pat_tb[NL80211_WOWLAN_PKTPAT_MASK]) !=
			    mask_len)
				goto error;
			if (pat_len > wowlan->pattern_max_len ||
			    pat_len < wowlan->pattern_min_len)
				goto error;

			new_triggers.patterns[i].mask =
				kmalloc(mask_len + pat_len, GFP_KERNEL);
			if (!new_triggers.patterns[i].mask) {
				err = -ENOMEM;
				goto error;
			}
			new_triggers.patterns[i].pattern =
				new_triggers.patterns[i].mask + mask_len;
			memcpy(new_triggers.patterns[i].mask,
			       nla_data(pat_tb[NL80211_WOWLAN_PKTPAT_MASK]),
			       mask_len);
			new_triggers.patterns[i].pattern_len = pat_len;
			memcpy(new_triggers.patterns[i].pattern,
			       nla_data(pat_tb[NL80211_WOWLAN_PKTPAT_PATTERN]),
			       pat_len);
			i++;
		}
	}

	if (memcmp(&new_triggers, &no_triggers, sizeof(new_triggers))) {
		struct cfg80211_wowlan *ntrig;
		ntrig = kmemdup(&new_triggers, sizeof(new_triggers),
				GFP_KERNEL);
		if (!ntrig) {
			err = -ENOMEM;
			goto error;
		}
		cfg80211_rdev_free_wowlan(rdev);
		rdev->wowlan = ntrig;
	} else {
 no_triggers:
		cfg80211_rdev_free_wowlan(rdev);
		rdev->wowlan = NULL;
	}

	return 0;
 error:
	for (i = 0; i < new_triggers.n_patterns; i++)
		kfree(new_triggers.patterns[i].mask);
	kfree(new_triggers.patterns);
	return err;
}