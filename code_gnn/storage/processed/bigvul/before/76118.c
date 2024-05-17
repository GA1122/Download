check_script_secure(notify_script_t *script,
#ifndef _HAVE_LIBMAGIC_
					     __attribute__((unused))
#endif
								     magic_t magic)
{
	int flags;
	int ret, ret_real, ret_new;
	struct stat file_buf, real_buf;
	bool need_script_protection = false;
	uid_t old_uid = 0;
	gid_t old_gid = 0;
	char *new_path;
	int sav_errno;
	char *real_file_path;
	char *orig_file_part, *new_file_part;

	if (!script)
		return 0;

	 
	if (script->args[0][0] == '<' &&
	    script->args[0][strspn(script->args[0] + 1, " \t") + 1] == '/')
		return SC_SYSTEM;

	if (!strchr(script->args[0], '/')) {
		 
		if ((ret = find_path(script))) {
			if (ret == EACCES)
				log_message(LOG_INFO, "Permissions failure for script %s in path - disabling", script->args[0]);
			else
				log_message(LOG_INFO, "Cannot find script %s in path - disabling", script->args[0]);
			return SC_NOTFOUND;
		}
	}

	 
	if (script->uid)
		old_uid = geteuid();
	if (script->gid)
		old_gid = getegid();

	if ((script->gid && setegid(script->gid)) ||
	    (script->uid && seteuid(script->uid))) {
		log_message(LOG_INFO, "Unable to set uid:gid %d:%d for script %s - disabling", script->uid, script->gid, script->args[0]);

		if ((script->uid && seteuid(old_uid)) ||
		    (script->gid && setegid(old_gid)))
			log_message(LOG_INFO, "Unable to restore uid:gid %d:%d after script %s", script->uid, script->gid, script->args[0]);

		return SC_INHIBIT;
	}

	 
	new_path = realpath(script->args[0], NULL);
	sav_errno = errno;

	if ((script->gid && setegid(old_gid)) ||
	    (script->uid && seteuid(old_uid)))
		log_message(LOG_INFO, "Unable to restore uid:gid %d:%d after checking script %s", script->uid, script->gid, script->args[0]);

	if (!new_path)
	{
		log_message(LOG_INFO, "Script %s cannot be accessed - %s", script->args[0], strerror(sav_errno));

		return SC_NOTFOUND;
	}

	real_file_path = NULL;

	orig_file_part = strrchr(script->args[0], '/');
	new_file_part = strrchr(new_path, '/');
	if (strcmp(script->args[0], new_path)) {
		 

		 
		if (strcmp(orig_file_part + 1, new_file_part + 1)) {
			real_file_path = new_path;
			new_path = MALLOC(new_file_part - real_file_path + 1 + strlen(orig_file_part + 1) + 1);
			strncpy(new_path, real_file_path, new_file_part + 1 - real_file_path);
			strcpy(new_path + (new_file_part + 1 - real_file_path), orig_file_part + 1);

			 
			ret_real = stat(real_file_path, &real_buf);
			ret_new = stat(new_path, &file_buf);
			if (!ret_real &&
			    (ret_new ||
			     real_buf.st_dev != file_buf.st_dev ||
			     real_buf.st_ino != file_buf.st_ino)) {
				 
				FREE(new_path);
				new_path = real_file_path;
				real_file_path = NULL;
			}
		}

		if (strcmp(script->args[0], new_path)) {
	 		 
			replace_cmd_name(script, new_path);
		}
	}

	if (!real_file_path)
		free(new_path);
	else
		FREE(new_path);

	 
	if (stat(real_file_path ? real_file_path : script->args[0], &file_buf)) {
		log_message(LOG_INFO, "Unable to access script `%s` - disabling", script->args[0]);
		return SC_NOTFOUND;
	}

	flags = SC_ISSCRIPT;

	 
	if (is_executable(&file_buf, script->uid, script->gid)) {
		flags |= SC_EXECUTABLE;
		if (script->uid == 0 || script->gid == 0 ||
		    (file_buf.st_uid == 0 && (file_buf.st_mode & S_IXUSR) && (file_buf.st_mode & S_ISUID)) ||
		    (file_buf.st_gid == 0 && (file_buf.st_mode & S_IXGRP) && (file_buf.st_mode & S_ISGID)))
			need_script_protection = true;
	} else
		log_message(LOG_INFO, "WARNING - script '%s' is not executable for uid:gid %d:%d - disabling.", script->args[0], script->uid, script->gid);

	 
	script->flags |= SC_EXECABLE;
#ifdef _HAVE_LIBMAGIC_
	if (magic && flags & SC_EXECUTABLE) {
		const char *magic_desc = magic_file(magic, real_file_path ? real_file_path : script->args[0]);
		if (!strstr(magic_desc, " executable") &&
		    !strstr(magic_desc, " shared object")) {
			log_message(LOG_INFO, "Please add a #! shebang to script %s", script->args[0]);
			script->flags &= ~SC_EXECABLE;
		}
	}
#endif

	if (!need_script_protection) {
		if (real_file_path)
			free(real_file_path);

		return flags;
	}

	 
	flags |= check_security(script->args[0], script_security);

	if (real_file_path) {
		flags |= check_security(real_file_path, script_security);
		free(real_file_path);
	}

	return flags;
}