uid_t from_kuid(struct user_namespace *targ, kuid_t kuid)
{
	 
	return map_id_up(&targ->uid_map, __kuid_val(kuid));
}
