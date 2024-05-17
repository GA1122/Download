static int srpt_handle_cmd(struct srpt_rdma_ch *ch,
			   struct srpt_recv_ioctx *recv_ioctx,
			   struct srpt_send_ioctx *send_ioctx)
{
	struct se_cmd *cmd;
	struct srp_cmd *srp_cmd;
	uint64_t unpacked_lun;
	u64 data_len;
	enum dma_data_direction dir;
	sense_reason_t ret;
	int rc;

	BUG_ON(!send_ioctx);

	srp_cmd = recv_ioctx->ioctx.buf;
	cmd = &send_ioctx->cmd;
	cmd->tag = srp_cmd->tag;

	switch (srp_cmd->task_attr) {
	case SRP_CMD_SIMPLE_Q:
		cmd->sam_task_attr = TCM_SIMPLE_TAG;
		break;
	case SRP_CMD_ORDERED_Q:
	default:
		cmd->sam_task_attr = TCM_ORDERED_TAG;
		break;
	case SRP_CMD_HEAD_OF_Q:
		cmd->sam_task_attr = TCM_HEAD_TAG;
		break;
	case SRP_CMD_ACA:
		cmd->sam_task_attr = TCM_ACA_TAG;
		break;
	}

	if (srpt_get_desc_tbl(send_ioctx, srp_cmd, &dir, &data_len)) {
		pr_err("0x%llx: parsing SRP descriptor table failed.\n",
		       srp_cmd->tag);
		ret = TCM_INVALID_CDB_FIELD;
		goto send_sense;
	}

	unpacked_lun = srpt_unpack_lun((uint8_t *)&srp_cmd->lun,
				       sizeof(srp_cmd->lun));
	rc = target_submit_cmd(cmd, ch->sess, srp_cmd->cdb,
			&send_ioctx->sense_data[0], unpacked_lun, data_len,
			TCM_SIMPLE_TAG, dir, TARGET_SCF_ACK_KREF);
	if (rc != 0) {
		ret = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		goto send_sense;
	}
	return 0;

send_sense:
	transport_send_check_condition_and_sense(cmd, ret, 0);
 	return -1;
 }