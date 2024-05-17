static int parse_audio_extension_unit(struct mixer_build *state, int unitid,
				      void *raw_desc)
{
	 
	return build_audio_procunit(state, unitid, raw_desc,
				    extunits, "Extension Unit");
}
