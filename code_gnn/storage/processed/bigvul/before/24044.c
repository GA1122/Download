static int mpi_init_descriptors (struct airo_info *ai)
{
	Cmd cmd;
	Resp rsp;
	int i;
	int rc = SUCCESS;

	 
	netif_stop_queue(ai->dev);

	memset(&rsp,0,sizeof(rsp));
	memset(&cmd,0,sizeof(cmd));

	cmd.cmd = CMD_ALLOCATEAUX;
	cmd.parm0 = FID_RX;
	cmd.parm1 = (ai->rxfids[0].card_ram_off - ai->pciaux);
	cmd.parm2 = MPI_MAX_FIDS;
	rc=issuecommand(ai, &cmd, &rsp);
	if (rc != SUCCESS) {
		airo_print_err(ai->dev->name, "Couldn't allocate RX FID");
		return rc;
	}

	for (i=0; i<MPI_MAX_FIDS; i++) {
		memcpy_toio(ai->rxfids[i].card_ram_off,
			&ai->rxfids[i].rx_desc, sizeof(RxFid));
	}

	 

	memset(&rsp,0,sizeof(rsp));
	memset(&cmd,0,sizeof(cmd));

	cmd.cmd = CMD_ALLOCATEAUX;
	cmd.parm0 = FID_TX;
	cmd.parm1 = (ai->txfids[0].card_ram_off - ai->pciaux);
	cmd.parm2 = MPI_MAX_FIDS;

	for (i=0; i<MPI_MAX_FIDS; i++) {
		ai->txfids[i].tx_desc.valid = 1;
		memcpy_toio(ai->txfids[i].card_ram_off,
			&ai->txfids[i].tx_desc, sizeof(TxFid));
	}
	ai->txfids[i-1].tx_desc.eoc = 1;  

	rc=issuecommand(ai, &cmd, &rsp);
	if (rc != SUCCESS) {
		airo_print_err(ai->dev->name, "Couldn't allocate TX FID");
		return rc;
	}

	 
	memset(&rsp,0,sizeof(rsp));
	memset(&cmd,0,sizeof(cmd));

	cmd.cmd = CMD_ALLOCATEAUX;
	cmd.parm0 = RID_RW;
	cmd.parm1 = (ai->config_desc.card_ram_off - ai->pciaux);
	cmd.parm2 = 1;  
	rc=issuecommand(ai, &cmd, &rsp);
	if (rc != SUCCESS) {
		airo_print_err(ai->dev->name, "Couldn't allocate RID");
		return rc;
	}

	memcpy_toio(ai->config_desc.card_ram_off,
		&ai->config_desc.rid_desc, sizeof(Rid));

	return rc;
}