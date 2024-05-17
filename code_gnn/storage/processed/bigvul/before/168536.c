static unsigned long get_ancestor_session_id(DWORD devinst, unsigned level)
{
	DWORD parent_devinst;
	unsigned long session_id = 0;
	char* sanitized_path = NULL;
	char path[MAX_PATH_LENGTH];
	unsigned i;

	if (level < 1) return 0;
	for (i = 0; i<level; i++) {
		if (CM_Get_Parent(&parent_devinst, devinst, 0) != CR_SUCCESS) {
			return 0;
		}
		devinst = parent_devinst;
	}
	if (CM_Get_Device_IDA(devinst, path, MAX_PATH_LENGTH, 0) != CR_SUCCESS) {
		return 0;
	}
	sanitized_path = sanitize_path(path);
	if (sanitized_path == NULL) {
		return 0;
	}
	session_id = htab_hash(sanitized_path);
	safe_free(sanitized_path);
	return session_id;
}
