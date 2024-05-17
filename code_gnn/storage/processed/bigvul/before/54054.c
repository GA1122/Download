static __be32 confirm_addr_indev(struct in_device *in_dev, __be32 dst,
			      __be32 local, int scope)
{
	int same = 0;
	__be32 addr = 0;

	for_ifa(in_dev) {
		if (!addr &&
		    (local == ifa->ifa_local || !local) &&
		    ifa->ifa_scope <= scope) {
			addr = ifa->ifa_local;
			if (same)
				break;
		}
		if (!same) {
			same = (!local || inet_ifa_match(local, ifa)) &&
				(!dst || inet_ifa_match(dst, ifa));
			if (same && addr) {
				if (local || !dst)
					break;
				 
				if (inet_ifa_match(addr, ifa))
					break;
				 
				if (ifa->ifa_scope <= scope) {
					addr = ifa->ifa_local;
					break;
				}
				 
				same = 0;
			}
		}
	} endfor_ifa(in_dev);

	return same ? addr : 0;
}
