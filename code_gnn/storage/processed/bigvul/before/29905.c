kgid_t make_kgid(struct user_namespace *ns, gid_t gid)
{
	 
	return KGIDT_INIT(map_id_down(&ns->gid_map, gid));
}
