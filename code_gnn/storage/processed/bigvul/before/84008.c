GF_Err gf_isom_hevc_config_update_ex(GF_ISOFile *the_file, u32 trackNumber, u32 DescriptionIndex, GF_HEVCConfig *cfg, u32 operand_type)
{
	u32 array_incomplete;
	GF_TrackBox *trak;
	GF_Err e;
	GF_MPEGVisualSampleEntryBox *entry;

	e = CanAccessMovie(the_file, GF_ISOM_OPEN_WRITE);
	if (e) return e;
	trak = gf_isom_get_track_from_file(the_file, trackNumber);
	if (!trak || !trak->Media || !DescriptionIndex) return GF_BAD_PARAM;
	entry = (GF_MPEGVisualSampleEntryBox *)gf_list_get(trak->Media->information->sampleTable->SampleDescription->other_boxes, DescriptionIndex-1);
	if (!entry) return GF_BAD_PARAM;
	switch (entry->type) {
	case GF_ISOM_BOX_TYPE_HVC1:
	case GF_ISOM_BOX_TYPE_HEV1:
	case GF_ISOM_BOX_TYPE_HVC2:
	case GF_ISOM_BOX_TYPE_HEV2:
	case GF_ISOM_BOX_TYPE_LHV1:
	case GF_ISOM_BOX_TYPE_LHE1:
	case GF_ISOM_BOX_TYPE_HVT1:
		break;
	default:
		return GF_BAD_PARAM;
	}


	if (operand_type == GF_ISOM_HVCC_SET_TILE_BASE_TRACK) {
		if (entry->type==GF_ISOM_BOX_TYPE_HVC1)
			entry->type = GF_ISOM_BOX_TYPE_HVC2;
		else if (entry->type==GF_ISOM_BOX_TYPE_HEV1)
			entry->type = GF_ISOM_BOX_TYPE_HEV2;
	} else if (operand_type == GF_ISOM_HVCC_SET_TILE) {
		if (!entry->hevc_config) entry->hevc_config = (GF_HEVCConfigurationBox*)gf_isom_box_new(GF_ISOM_BOX_TYPE_HVCC);
		if (entry->hevc_config->config) gf_odf_hevc_cfg_del(entry->hevc_config->config);
		entry->hevc_config->config = NULL;
		entry->type = GF_ISOM_BOX_TYPE_HVT1;
	} else if (operand_type < GF_ISOM_HVCC_SET_LHVC) {
		if ((operand_type != GF_ISOM_HVCC_SET_INBAND) && !entry->hevc_config)
			entry->hevc_config = (GF_HEVCConfigurationBox*)gf_isom_box_new(GF_ISOM_BOX_TYPE_HVCC);

		if (cfg) {
			if (entry->hevc_config->config) gf_odf_hevc_cfg_del(entry->hevc_config->config);
			entry->hevc_config->config = HEVC_DuplicateConfig(cfg);
		} else if (operand_type != GF_ISOM_HVCC_SET_TILE) {
			operand_type=GF_ISOM_HVCC_SET_INBAND;
		}
		array_incomplete = (operand_type==GF_ISOM_HVCC_SET_INBAND) ? 1 : 0;
		if (entry->hevc_config && hevc_cleanup_config(entry->hevc_config->config, operand_type))
			array_incomplete=1;

		if (entry->lhvc_config && hevc_cleanup_config(entry->lhvc_config->config, operand_type))
			array_incomplete=1;

		switch (entry->type) {
		case GF_ISOM_BOX_TYPE_HEV1:
		case GF_ISOM_BOX_TYPE_HVC1:
			entry->type = array_incomplete ? GF_ISOM_BOX_TYPE_HEV1 : GF_ISOM_BOX_TYPE_HVC1;
			break;
		case GF_ISOM_BOX_TYPE_HEV2:
		case GF_ISOM_BOX_TYPE_HVC2:
			entry->type = array_incomplete ? GF_ISOM_BOX_TYPE_HEV2 : GF_ISOM_BOX_TYPE_HVC2;
			break;
		case GF_ISOM_BOX_TYPE_LHE1:
		case GF_ISOM_BOX_TYPE_LHV1:
			entry->type = array_incomplete ? GF_ISOM_BOX_TYPE_LHE1 : GF_ISOM_BOX_TYPE_LHV1;
			break;
		}
	} else {

		 
		if ((operand_type==GF_ISOM_HVCC_SET_LHVC_WITH_BASE_BACKWARD) || (operand_type==GF_ISOM_HVCC_SET_LHVC_WITH_BASE)) {
			if (!entry->hevc_config) return GF_BAD_PARAM;
			if (!cfg) {
				if (entry->lhvc_config) {
					gf_isom_box_del((GF_Box*)entry->lhvc_config);
					entry->lhvc_config = NULL;
				}
				if (entry->type==GF_ISOM_BOX_TYPE_LHE1) entry->type = (operand_type==GF_ISOM_HVCC_SET_LHVC_WITH_BASE) ? GF_ISOM_BOX_TYPE_HEV2 : GF_ISOM_BOX_TYPE_HEV1;
				else if (entry->type==GF_ISOM_BOX_TYPE_HEV1) entry->type = (operand_type==GF_ISOM_HVCC_SET_LHVC_WITH_BASE) ? GF_ISOM_BOX_TYPE_HEV2 : GF_ISOM_BOX_TYPE_HEV1;
				else entry->type =  (operand_type==GF_ISOM_HVCC_SET_LHVC_WITH_BASE) ? GF_ISOM_BOX_TYPE_HVC2 : GF_ISOM_BOX_TYPE_HVC1;
			} else {
				if (!entry->lhvc_config) entry->lhvc_config = (GF_HEVCConfigurationBox*)gf_isom_box_new(GF_ISOM_BOX_TYPE_LHVC);
				if (entry->lhvc_config->config) gf_odf_hevc_cfg_del(entry->lhvc_config->config);
				entry->lhvc_config->config = HEVC_DuplicateConfig(cfg);

				if (operand_type==GF_ISOM_HVCC_SET_LHVC_WITH_BASE_BACKWARD) {
					if (entry->type==GF_ISOM_BOX_TYPE_HEV2) entry->type = GF_ISOM_BOX_TYPE_HEV1;
					else entry->type = GF_ISOM_BOX_TYPE_HVC1;
				} else {
					if (entry->type==GF_ISOM_BOX_TYPE_HEV1) entry->type = GF_ISOM_BOX_TYPE_HEV2;
					else entry->type = GF_ISOM_BOX_TYPE_HVC2;
				}
			}
		}
		 
		else if (operand_type==GF_ISOM_HVCC_SET_LHVC) {
			if (entry->hevc_config) {
				gf_isom_box_del((GF_Box*)entry->hevc_config);
				entry->hevc_config=NULL;
			}
			if (!cfg) return GF_BAD_PARAM;

			if (!entry->lhvc_config) entry->lhvc_config = (GF_HEVCConfigurationBox*)gf_isom_box_new(GF_ISOM_BOX_TYPE_LHVC);
			if (entry->lhvc_config->config) gf_odf_hevc_cfg_del(entry->lhvc_config->config);
			entry->lhvc_config->config = HEVC_DuplicateConfig(cfg);

			if ((entry->type==GF_ISOM_BOX_TYPE_HEV1) || (entry->type==GF_ISOM_BOX_TYPE_HEV2)) entry->type = GF_ISOM_BOX_TYPE_LHE1;
			else entry->type = GF_ISOM_BOX_TYPE_LHV1;
		}
		 
		else if (operand_type==GF_ISOM_LHCC_SET_INBAND) {
			entry->type = GF_ISOM_BOX_TYPE_LHE1;
		}
	}

	HEVC_RewriteESDescriptor(entry);
	return GF_OK;
}