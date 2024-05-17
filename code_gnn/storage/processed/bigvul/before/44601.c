int lxc_map_ids(struct lxc_list *idmap, pid_t pid)
{
	struct lxc_list *iterator;
	struct id_map *map;
	int ret = 0, use_shadow = 0;
	enum idtype type;
	char *buf = NULL, *pos, *cmdpath = NULL;

	 
	cmdpath = on_path("newuidmap", NULL);
	if (cmdpath) {
		use_shadow = 1;
		free(cmdpath);
	}

	if (!use_shadow && geteuid()) {
		ERROR("Missing newuidmap/newgidmap");
		return -1;
	}

	for(type = ID_TYPE_UID; type <= ID_TYPE_GID; type++) {
		int left, fill;
		int had_entry = 0;
		if (!buf) {
			buf = pos = malloc(4096);
			if (!buf)
				return -ENOMEM;
		}
		pos = buf;
		if (use_shadow)
			pos += sprintf(buf, "new%cidmap %d",
				type == ID_TYPE_UID ? 'u' : 'g',
				pid);

		lxc_list_for_each(iterator, idmap) {
			 
			map = iterator->elem;
			if (map->idtype != type)
				continue;

			had_entry = 1;
			left = 4096 - (pos - buf);
			fill = snprintf(pos, left, "%s%lu %lu %lu%s",
					use_shadow ? " " : "",
					map->nsid, map->hostid, map->range,
					use_shadow ? "" : "\n");
			if (fill <= 0 || fill >= left)
				SYSERROR("snprintf failed, too many mappings");
			pos += fill;
		}
		if (!had_entry)
			continue;

		if (!use_shadow) {
			ret = write_id_mapping(type, pid, buf, pos-buf);
		} else {
			left = 4096 - (pos - buf);
			fill = snprintf(pos, left, "\n");
			if (fill <= 0 || fill >= left)
				SYSERROR("snprintf failed, too many mappings");
			pos += fill;
			ret = system(buf);
		}

		if (ret)
			break;
	}

	free(buf);
	return ret;
}