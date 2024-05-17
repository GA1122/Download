void sas_ata_eh(struct Scsi_Host *shost, struct list_head *work_q,
		struct list_head *done_q)
{
	struct scsi_cmnd *cmd, *n;
	struct domain_device *eh_dev;

	do {
		LIST_HEAD(sata_q);
		eh_dev = NULL;

		list_for_each_entry_safe(cmd, n, work_q, eh_entry) {
			struct domain_device *ddev = cmd_to_domain_dev(cmd);

			if (!dev_is_sata(ddev) || TO_SAS_TASK(cmd))
				continue;
			if (eh_dev && eh_dev != ddev)
				continue;
			eh_dev = ddev;
			list_move(&cmd->eh_entry, &sata_q);
		}

		if (!list_empty(&sata_q)) {
			struct ata_port *ap = eh_dev->sata_dev.ap;

			sas_ata_printk(KERN_DEBUG, eh_dev, "cmd error handler\n");
			ata_scsi_cmd_error_handler(shost, ap, &sata_q);
			 
			while (!list_empty(&sata_q))
				list_del_init(sata_q.next);
		}
	} while (eh_dev);
}
