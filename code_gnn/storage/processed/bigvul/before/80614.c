GF_Box *trun_New()
{
	ISOM_DECL_BOX_ALLOC(GF_TrackFragmentRunBox, GF_ISOM_BOX_TYPE_TRUN);
	tmp->entries = gf_list_new();
	return (GF_Box *)tmp;
}