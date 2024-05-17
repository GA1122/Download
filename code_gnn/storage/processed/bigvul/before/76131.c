notify_script_init(int extra_params, const char *type)
{
	notify_script_t *script = MALLOC(sizeof(notify_script_t));
	vector_t *strvec_qe;

	 
	strvec_qe = alloc_strvec_quoted_escaped(NULL);

	if (!strvec_qe) {
		log_message(LOG_INFO, "Unable to parse notify script");
		FREE(script);
		return NULL;
	}

	set_script_params_array(strvec_qe, script, extra_params);
	if (!script->args) {
		log_message(LOG_INFO, "Unable to parse script '%s' - ignoring", FMT_STR_VSLOT(strvec_qe, 1));
		FREE(script);
		free_strvec(strvec_qe);
		return NULL;
	}

	script->flags = 0;

	if (vector_size(strvec_qe) > 2) {
		if (set_script_uid_gid(strvec_qe, 2, &script->uid, &script->gid)) {
			log_message(LOG_INFO, "Invalid user/group for %s script %s - ignoring", type, script->args[0]);
			FREE(script->args);
			FREE(script);
			free_strvec(strvec_qe);
			return NULL;
		}
	}
	else {
		if (set_default_script_user(NULL, NULL)) {
			log_message(LOG_INFO, "Failed to set default user for %s script %s - ignoring", type, script->args[0]);
			FREE(script->args);
			FREE(script);
			free_strvec(strvec_qe);
			return NULL;
		}

		script->uid = default_script_uid;
		script->gid = default_script_gid;
	}

	free_strvec(strvec_qe);

	return script;
}