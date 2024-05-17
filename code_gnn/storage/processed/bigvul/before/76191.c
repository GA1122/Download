static bool new_idmap_permitted(const struct file *file,
				struct user_namespace *ns, int cap_setid,
				struct uid_gid_map *new_map)
{
	const struct cred *cred = file->f_cred;
	 
	if ((new_map->nr_extents == 1) && (new_map->extent[0].count == 1) &&
	    uid_eq(ns->owner, cred->euid)) {
		u32 id = new_map->extent[0].lower_first;
		if (cap_setid == CAP_SETUID) {
			kuid_t uid = make_kuid(ns->parent, id);
			if (uid_eq(uid, cred->euid))
				return true;
		} else if (cap_setid == CAP_SETGID) {
			kgid_t gid = make_kgid(ns->parent, id);
			if (!(ns->flags & USERNS_SETGROUPS_ALLOWED) &&
			    gid_eq(gid, cred->egid))
				return true;
		}
	}

	 
	if (!cap_valid(cap_setid))
		return true;

	 
	if (ns_capable(ns->parent, cap_setid) &&
	    file_ns_capable(file, ns->parent, cap_setid))
		return true;

	return false;
}