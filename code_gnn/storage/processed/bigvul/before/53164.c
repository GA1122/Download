static size_t rtnl_link_get_af_size(const struct net_device *dev,
				    u32 ext_filter_mask)
{
	struct rtnl_af_ops *af_ops;
	size_t size;

	 
	size = nla_total_size(sizeof(struct nlattr));

	list_for_each_entry(af_ops, &rtnl_af_ops, list) {
		if (af_ops->get_link_af_size) {
			 
			size += nla_total_size(sizeof(struct nlattr)) +
				af_ops->get_link_af_size(dev, ext_filter_mask);
		}
	}

	return size;
}
