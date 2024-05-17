static inline bool abs_cgroup_supported(void) {
	return api_version >= CGM_SUPPORTS_GET_ABS;
}
