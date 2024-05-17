megasas_register_aen(struct megasas_instance *instance, u32 seq_num,
		     u32 class_locale_word)
{
	int ret_val;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	union megasas_evt_class_locale curr_aen;
	union megasas_evt_class_locale prev_aen;

	 

	curr_aen.word = class_locale_word;

	if (instance->aen_cmd) {

		prev_aen.word =
			le32_to_cpu(instance->aen_cmd->frame->dcmd.mbox.w[1]);

		if ((curr_aen.members.class < MFI_EVT_CLASS_DEBUG) ||
		    (curr_aen.members.class > MFI_EVT_CLASS_DEAD)) {
			dev_info(&instance->pdev->dev,
				 "%s %d out of range class %d send by application\n",
				 __func__, __LINE__, curr_aen.members.class);
			return 0;
		}

		 
		if ((prev_aen.members.class <= curr_aen.members.class) &&
		    !((prev_aen.members.locale & curr_aen.members.locale) ^
		      curr_aen.members.locale)) {
			 
			return 0;
		} else {
			curr_aen.members.locale |= prev_aen.members.locale;

			if (prev_aen.members.class < curr_aen.members.class)
				curr_aen.members.class = prev_aen.members.class;

			instance->aen_cmd->abort_aen = 1;
			ret_val = megasas_issue_blocked_abort_cmd(instance,
								  instance->
								  aen_cmd, 30);

			if (ret_val) {
				dev_printk(KERN_DEBUG, &instance->pdev->dev, "Failed to abort "
				       "previous AEN command\n");
				return ret_val;
			}
		}
	}

	cmd = megasas_get_cmd(instance);

	if (!cmd)
		return -ENOMEM;

	dcmd = &cmd->frame->dcmd;

	memset(instance->evt_detail, 0, sizeof(struct megasas_evt_detail));

	 
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = 0x0;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->pad_0 = 0;
	dcmd->data_xfer_len = cpu_to_le32(sizeof(struct megasas_evt_detail));
	dcmd->opcode = cpu_to_le32(MR_DCMD_CTRL_EVENT_WAIT);
	dcmd->mbox.w[0] = cpu_to_le32(seq_num);
	instance->last_seq_num = seq_num;
	dcmd->mbox.w[1] = cpu_to_le32(curr_aen.word);

	megasas_set_dma_settings(instance, dcmd, instance->evt_detail_h,
				 sizeof(struct megasas_evt_detail));

	if (instance->aen_cmd != NULL) {
		megasas_return_cmd(instance, cmd);
		return 0;
	}

	 
	instance->aen_cmd = cmd;

	 
	instance->instancet->issue_dcmd(instance, cmd);

	return 0;
}