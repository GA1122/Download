static int parse_tag(blkid_cache cache, blkid_dev dev, char **cp)
{
	char *name = NULL;
	char *value = NULL;
	int ret;

	if (!cache || !dev)
		return -BLKID_ERR_PARAM;

	if ((ret = parse_token(&name, &value, cp)) <= 0  )
		return ret;

	 
	if (!strcmp(name, "DEVNO"))
		dev->bid_devno = STRTOULL(value, 0, 0);
	else if (!strcmp(name, "PRI"))
		dev->bid_pri = strtol(value, 0, 0);
	else if (!strcmp(name, "TIME")) {
		char *end = NULL;
		dev->bid_time = STRTOULL(value, &end, 0);
		if (end && *end == '.')
			dev->bid_utime = STRTOULL(end + 1, 0, 0);
	} else
		ret = blkid_set_tag(dev, name, value, strlen(value));

	DBG(READ, ul_debug("    tag: %s=\"%s\"", name, value));

	return ret < 0 ? ret : 1;
}