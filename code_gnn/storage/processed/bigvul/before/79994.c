void audio_sample_entry_del(GF_Box *s)
{
	GF_MPEGAudioSampleEntryBox *ptr = (GF_MPEGAudioSampleEntryBox *)s;
	if (ptr == NULL) return;
	gf_isom_sample_entry_predestroy((GF_SampleEntryBox *)s);

	if (ptr->esd) gf_isom_box_del((GF_Box *)ptr->esd);
	if (ptr->slc) gf_odf_desc_del((GF_Descriptor *)ptr->slc);
	if (ptr->cfg_ac3) gf_isom_box_del((GF_Box *)ptr->cfg_ac3);
	if (ptr->cfg_3gpp) gf_isom_box_del((GF_Box *)ptr->cfg_3gpp);
	gf_free(ptr);
}