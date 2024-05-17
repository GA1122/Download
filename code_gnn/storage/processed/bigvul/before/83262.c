static void sas_end_task(struct scsi_cmnd *sc, struct sas_task *task)
{
	struct task_status_struct *ts = &task->task_status;
	int hs = 0, stat = 0;

	if (ts->resp == SAS_TASK_UNDELIVERED) {
		 
		hs = DID_NO_CONNECT;
	} else {  
		 
		switch (ts->stat) {
		case SAS_DEV_NO_RESPONSE:
		case SAS_INTERRUPTED:
		case SAS_PHY_DOWN:
		case SAS_NAK_R_ERR:
		case SAS_OPEN_TO:
			hs = DID_NO_CONNECT;
			break;
		case SAS_DATA_UNDERRUN:
			scsi_set_resid(sc, ts->residual);
			if (scsi_bufflen(sc) - scsi_get_resid(sc) < sc->underflow)
				hs = DID_ERROR;
			break;
		case SAS_DATA_OVERRUN:
			hs = DID_ERROR;
			break;
		case SAS_QUEUE_FULL:
			hs = DID_SOFT_ERROR;  
			break;
		case SAS_DEVICE_UNKNOWN:
			hs = DID_BAD_TARGET;
			break;
		case SAS_SG_ERR:
			hs = DID_PARITY;
			break;
		case SAS_OPEN_REJECT:
			if (ts->open_rej_reason == SAS_OREJ_RSVD_RETRY)
				hs = DID_SOFT_ERROR;  
			else
				hs = DID_ERROR;
			break;
		case SAS_PROTO_RESPONSE:
			SAS_DPRINTK("LLDD:%s sent SAS_PROTO_RESP for an SSP "
				    "task; please report this\n",
				    task->dev->port->ha->sas_ha_name);
			break;
		case SAS_ABORTED_TASK:
			hs = DID_ABORT;
			break;
		case SAM_STAT_CHECK_CONDITION:
			memcpy(sc->sense_buffer, ts->buf,
			       min(SCSI_SENSE_BUFFERSIZE, ts->buf_valid_size));
			stat = SAM_STAT_CHECK_CONDITION;
			break;
		default:
			stat = ts->stat;
			break;
		}
	}

	sc->result = (hs << 16) | stat;
	ASSIGN_SAS_TASK(sc, NULL);
	sas_free_task(task);
}