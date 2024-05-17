static int tcm_loop_new_cmd_map(struct se_cmd *se_cmd)
{
	struct tcm_loop_cmd *tl_cmd = container_of(se_cmd,
				struct tcm_loop_cmd, tl_se_cmd);
	struct scsi_cmnd *sc = tl_cmd->sc;
	struct scatterlist *sgl_bidi = NULL;
	u32 sgl_bidi_count = 0;
	int ret;
	 
	ret = transport_generic_allocate_tasks(se_cmd, sc->cmnd);
	if (ret == -ENOMEM) {
		 
		return PYX_TRANSPORT_LU_COMM_FAILURE;
	} else if (ret == -EINVAL) {
		 
		if (se_cmd->se_cmd_flags & SCF_SCSI_RESERVATION_CONFLICT)
			return PYX_TRANSPORT_RESERVATION_CONFLICT;
		 
		return PYX_TRANSPORT_USE_SENSE_REASON;
	}

	 
	if (se_cmd->t_tasks_bidi) {
		struct scsi_data_buffer *sdb = scsi_in(sc);

		sgl_bidi = sdb->table.sgl;
		sgl_bidi_count = sdb->table.nents;
	}

	 
	ret = transport_generic_map_mem_to_cmd(se_cmd, scsi_sglist(sc),
			scsi_sg_count(sc), sgl_bidi, sgl_bidi_count);
	if (ret < 0)
		return PYX_TRANSPORT_LU_COMM_FAILURE;

	return 0;
}