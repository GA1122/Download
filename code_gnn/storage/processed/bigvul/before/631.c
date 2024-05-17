static bool ldap_decode_control_value(void *mem_ctx, DATA_BLOB value,
				      const struct ldap_control_handler *handlers,
				      struct ldb_control *ctrl)
{
	int i;

	if (!handlers) {
		return true;
	}

	for (i = 0; handlers[i].oid != NULL; i++) {
		if (strcmp(handlers[i].oid, ctrl->oid) == 0) {
			if (!handlers[i].decode || !handlers[i].decode(mem_ctx, value, &ctrl->data)) {
				return false;
			}
			break;
		}
	}
	if (handlers[i].oid == NULL) {
		return false;
	}

	return true;
}
