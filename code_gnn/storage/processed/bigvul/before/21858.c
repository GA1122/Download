DRM_ENUM_NAME_FN(drm_get_dirty_info_name,
		 drm_dirty_info_enum_list)

struct drm_conn_prop_enum_list {
	int type;
	char *name;
	int count;
};
