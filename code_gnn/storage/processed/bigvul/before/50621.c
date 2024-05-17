static int srpt_abort_cmd(struct srpt_send_ioctx *ioctx)
{
	enum srpt_command_state state;
	unsigned long flags;

	BUG_ON(!ioctx);

	 

	spin_lock_irqsave(&ioctx->spinlock, flags);
	state = ioctx->state;
	switch (state) {
	case SRPT_STATE_NEED_DATA:
		ioctx->state = SRPT_STATE_DATA_IN;
		break;
	case SRPT_STATE_DATA_IN:
	case SRPT_STATE_CMD_RSP_SENT:
	case SRPT_STATE_MGMT_RSP_SENT:
		ioctx->state = SRPT_STATE_DONE;
		break;
	default:
		break;
	}
	spin_unlock_irqrestore(&ioctx->spinlock, flags);

	if (state == SRPT_STATE_DONE) {
		struct srpt_rdma_ch *ch = ioctx->ch;

		BUG_ON(ch->sess == NULL);

		target_put_sess_cmd(&ioctx->cmd);
		goto out;
	}

	pr_debug("Aborting cmd with state %d and tag %lld\n", state,
		 ioctx->cmd.tag);

	switch (state) {
	case SRPT_STATE_NEW:
	case SRPT_STATE_DATA_IN:
	case SRPT_STATE_MGMT:
		 
		WARN_ON(!transport_check_aborted_status(&ioctx->cmd, false));
		break;
	case SRPT_STATE_NEED_DATA:
		 

		 
		spin_lock_irqsave(&ioctx->cmd.t_state_lock, flags);
		ioctx->cmd.transport_state &= ~CMD_T_ACTIVE;
		spin_unlock_irqrestore(&ioctx->cmd.t_state_lock, flags);
		break;
	case SRPT_STATE_CMD_RSP_SENT:
		 
		srpt_unmap_sg_to_ib_sge(ioctx->ch, ioctx);
		target_put_sess_cmd(&ioctx->cmd);
		break;
	case SRPT_STATE_MGMT_RSP_SENT:
		srpt_set_cmd_state(ioctx, SRPT_STATE_DONE);
		target_put_sess_cmd(&ioctx->cmd);
		break;
	default:
		WARN(1, "Unexpected command state (%d)", state);
		break;
	}

out:
	return state;
}
