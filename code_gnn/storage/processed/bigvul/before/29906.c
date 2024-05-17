kprojid_t make_kprojid(struct user_namespace *ns, projid_t projid)
{
	 
	return KPROJIDT_INIT(map_id_down(&ns->projid_map, projid));
}
