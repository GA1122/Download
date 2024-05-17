is_valid_shared_exe( const char *name )
{
	if ((strlen(name) < 4) || (strncmp(name, "exe-", 4) != 0)) {
		return FALSE;
	}
	MyString path;
	path.sprintf("%s/%s", Spool, name);
	int count = link_count(path.Value());
	if (count == 1) {
		return FALSE;
	}
	if (count == -1) {
		dprintf(D_ALWAYS, "link_count error on %s; not deleting\n", name);
	}
	return TRUE;
}
