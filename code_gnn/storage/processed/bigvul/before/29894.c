gid_t from_kgid(struct user_namespace *targ, kgid_t kgid)
{
	 
	return map_id_up(&targ->gid_map, __kgid_val(kgid));
}
