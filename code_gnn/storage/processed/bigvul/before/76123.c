find_path(notify_script_t *script)
{
	size_t filename_len;
	size_t file_len;
	size_t path_len;
	char *file = script->args[0];
	struct stat buf;
	int ret;
	int ret_val = ENOENT;
	int sgid_num;
	gid_t *sgid_list = NULL;
	const char *subp;
	bool got_eacces = false;
	const char *p;

	 
	if (*file == '\0')
		return ENOENT;

	filename_len = strlen(file);
	if (filename_len >= PATH_MAX) {
		ret_val = ENAMETOOLONG;
		goto exit1;
	}

	 
	if (strchr (file, '/') != NULL) {
		ret_val = 0;
		goto exit1;
	}

	 
	if (!path) {
		path = getenv ("PATH");

		if (!path) {
			size_t cs_path_len;
			path = MALLOC(cs_path_len = confstr(_CS_PATH, NULL, 0));
			confstr(_CS_PATH, path, cs_path_len);
			path_is_malloced = true;
		}
	}

	 
	file_len = strnlen (file, NAME_MAX + 1);
	path_len = strnlen (path, PATH_MAX - 1) + 1;

	if (file_len > NAME_MAX) {
		ret_val = ENAMETOOLONG;
		goto exit1;
	}

	 
	if (script->gid) {
		if (setegid(script->gid)) {
			log_message(LOG_INFO, "Unable to set egid to %d (%m)", script->gid);
			ret_val = EACCES;
			goto exit1;
		}

		 
		sgid_num = getgroups(0, NULL);
		sgid_list = MALLOC(((size_t)sgid_num + 1) * sizeof(gid_t));
		sgid_num = getgroups(sgid_num, sgid_list);
		sgid_list[sgid_num++] = 0;

		 
		if (setgroups(1, &script->gid)) {
			log_message(LOG_INFO, "Unable to set supplementary gids (%m)");
			ret_val = EACCES;
			goto exit;
		}
	}
	if (script->uid && seteuid(script->uid)) {
		log_message(LOG_INFO, "Unable to set euid to %d (%m)", script->uid);
		ret_val = EACCES;
		goto exit;
	}

	for (p = path; ; p = subp)
	{
		char buffer[path_len + file_len + 1];

		subp = strchrnul (p, ':');

		 
		if (subp >= p + path_len) {
			 
			if (*subp == '\0') {
				ret_val = ENOENT;
				goto exit;
			}

			 
			continue;
		}

		 
		char *pend = mempcpy (buffer, p, (size_t)(subp - p));
		*pend = '/';
		memcpy (pend + (p < subp), file, file_len + 1);

		ret = stat (buffer, &buf);
		if (!ret) {
			if (!S_ISREG(buf.st_mode))
				errno = EACCES;
			else if (!is_executable(&buf, script->uid, script->gid)) {
				errno = EACCES;
			} else {
				 
				log_message(LOG_INFO, "WARNING - script `%s` resolved by path search to `%s`. Please specify full path.", script->args[0], buffer);

				 
				replace_cmd_name(script, buffer);

				ret_val = 0;
				got_eacces = false;
				goto exit;
			}
		}

		switch (errno)
		{
		case ENOEXEC:
		case EACCES:
			 
			if (!ret)
				got_eacces = true;
		case ENOENT:
		case ESTALE:
		case ENOTDIR:
			 
		case ENODEV:
		case ETIMEDOUT:
			 
			break;

		default:
			 
			ret_val = -1;
			goto exit;
		}

		if (*subp++ == '\0')
			break;
	}

exit:
	 
	if (script->uid && seteuid(0))
		log_message(LOG_INFO, "Unable to restore euid after script search (%m)");
	if (script->gid) {
		if (setegid(0))
			log_message(LOG_INFO, "Unable to restore egid after script search (%m)");

		 
		if (sgid_list) {
			if (setgroups((size_t)sgid_num, sgid_list))
				log_message(LOG_INFO, "Unable to restore supplementary groups after script search (%m)");
			FREE(sgid_list);
		}
	}

exit1:
	 
	if (got_eacces) {
		 
		return EACCES;
	}

	return ret_val;
}
