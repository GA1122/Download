GF_Box *metx_New()
{
	ISOM_DECL_BOX_ALLOC(GF_MetaDataSampleEntryBox, GF_ISOM_BOX_TYPE_METX);
	gf_isom_sample_entry_init((GF_SampleEntryBox*)tmp);
	return (GF_Box *)tmp;
}