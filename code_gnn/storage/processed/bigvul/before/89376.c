int blk_get_device_part_str(const char *ifname, const char *dev_part_str,
			     struct blk_desc **dev_desc,
			     disk_partition_t *info, int allow_whole_dev)
{
	int ret = -1;
	const char *part_str;
	char *dup_str = NULL;
	const char *dev_str;
	int dev;
	char *ep;
	int p;
	int part;
	disk_partition_t tmpinfo;

#ifdef CONFIG_SANDBOX
	 
	if (0 == strcmp(ifname, "hostfs")) {
		*dev_desc = NULL;
		info->start = 0;
		info->size = 0;
		info->blksz = 0;
		info->bootable = 0;
		strcpy((char *)info->type, BOOT_PART_TYPE);
		strcpy((char *)info->name, "Sandbox host");
#if CONFIG_IS_ENABLED(PARTITION_UUIDS)
		info->uuid[0] = 0;
#endif
#ifdef CONFIG_PARTITION_TYPE_GUID
		info->type_guid[0] = 0;
#endif

		return 0;
	}
#endif

#ifdef CONFIG_CMD_UBIFS
	 
	if (0 == strcmp(ifname, "ubi")) {
		if (!ubifs_is_mounted()) {
			printf("UBIFS not mounted, use ubifsmount to mount volume first!\n");
			return -1;
		}

		*dev_desc = NULL;
		memset(info, 0, sizeof(*info));
		strcpy((char *)info->type, BOOT_PART_TYPE);
		strcpy((char *)info->name, "UBI");
#if CONFIG_IS_ENABLED(PARTITION_UUIDS)
		info->uuid[0] = 0;
#endif
		return 0;
	}
#endif

	 
	if (!dev_part_str || !strlen(dev_part_str) ||
	    !strcmp(dev_part_str, "-"))
		dev_part_str = env_get("bootdevice");

	 
	if (!dev_part_str) {
		printf("** No device specified **\n");
		goto cleanup;
	}

	 
	part_str = strchr(dev_part_str, ':');
	if (part_str) {
		dup_str = strdup(dev_part_str);
		dup_str[part_str - dev_part_str] = 0;
		dev_str = dup_str;
		part_str++;
	} else {
		dev_str = dev_part_str;
	}

	 
	dev = blk_get_device_by_str(ifname, dev_str, dev_desc);
	if (dev < 0)
		goto cleanup;

	 
	if (!part_str || !*part_str) {
		part = PART_UNSPECIFIED;
	} else if (!strcmp(part_str, "auto")) {
		part = PART_AUTO;
	} else {
		 
		part = (int)simple_strtoul(part_str, &ep, 16);
		 
		if (*ep || (part == 0 && !allow_whole_dev)) {
			printf("** Bad partition specification %s %s **\n",
			    ifname, dev_part_str);
			goto cleanup;
		}
	}

	 
	if (((*dev_desc)->part_type == PART_TYPE_UNKNOWN) ||
	    (part == 0)) {
		if (!(*dev_desc)->lba) {
			printf("** Bad device size - %s %s **\n", ifname,
			       dev_str);
			goto cleanup;
		}

		 
		if ((part > 0) || (!allow_whole_dev)) {
			printf("** No partition table - %s %s **\n", ifname,
			       dev_str);
			goto cleanup;
		}

		(*dev_desc)->log2blksz = LOG2((*dev_desc)->blksz);

		part_get_info_whole_disk(*dev_desc, info);

		ret = 0;
		goto cleanup;
	}

	 
	if (part == PART_UNSPECIFIED)
		part = 1;

	 
	if (part != PART_AUTO) {
		ret = part_get_info(*dev_desc, part, info);
		if (ret) {
			printf("** Invalid partition %d **\n", part);
			goto cleanup;
		}
	} else {
		 
		part = 0;
		for (p = 1; p <= MAX_SEARCH_PARTITIONS; p++) {
			ret = part_get_info(*dev_desc, p, info);
			if (ret)
				continue;

			 
			if (!part || info->bootable)
				part = p;

			 
			if (info->bootable)
				break;

			 
			if (part == p)
				tmpinfo = *info;
		}
		 
		if (part) {
			 
			if (p == MAX_SEARCH_PARTITIONS + 1)
				*info = tmpinfo;
		} else {
			printf("** No valid partitions found **\n");
			ret = -1;
			goto cleanup;
		}
	}
	if (strncmp((char *)info->type, BOOT_PART_TYPE, sizeof(info->type)) != 0) {
		printf("** Invalid partition type \"%.32s\""
			" (expect \"" BOOT_PART_TYPE "\")\n",
			info->type);
		ret  = -1;
		goto cleanup;
	}

	(*dev_desc)->log2blksz = LOG2((*dev_desc)->blksz);

	ret = part;
	goto cleanup;

cleanup:
	free(dup_str);
	return ret;
}
