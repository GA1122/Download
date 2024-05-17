GF_Box *sgpd_New()
{
	ISOM_DECL_BOX_ALLOC(GF_SampleGroupDescriptionBox, GF_ISOM_BOX_TYPE_SGPD);
	 
	tmp->version = 1;
	tmp->group_descriptions = gf_list_new();
	return (GF_Box *)tmp;
}
