projid_t from_kprojid(struct user_namespace *targ, kprojid_t kprojid)
{
	 
	return map_id_up(&targ->projid_map, __kprojid_val(kprojid));
}
