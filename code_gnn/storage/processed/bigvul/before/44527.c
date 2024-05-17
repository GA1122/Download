static bool cgm_mount_cgroup(void *hdata, const char *root, int type)
{
	if (dir_exists(CGMANAGER_LOWER_SOCK))
		return cgm_bind_dir(root, CGMANAGER_LOWER_SOCK);
	if (dir_exists(CGMANAGER_UPPER_SOCK))
		return cgm_bind_dir(root, CGMANAGER_UPPER_SOCK);
	return false;
}