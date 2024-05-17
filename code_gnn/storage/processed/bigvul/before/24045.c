static int mpi_map_card(struct airo_info *ai, struct pci_dev *pci)
{
	unsigned long mem_start, mem_len, aux_start, aux_len;
	int rc = -1;
	int i;
	dma_addr_t busaddroff;
	unsigned char *vpackoff;
	unsigned char __iomem *pciaddroff;

	mem_start = pci_resource_start(pci, 1);
	mem_len = pci_resource_len(pci, 1);
	aux_start = pci_resource_start(pci, 2);
	aux_len = AUXMEMSIZE;

	if (!request_mem_region(mem_start, mem_len, DRV_NAME)) {
		airo_print_err("", "Couldn't get region %x[%x]",
			(int)mem_start, (int)mem_len);
		goto out;
	}
	if (!request_mem_region(aux_start, aux_len, DRV_NAME)) {
		airo_print_err("", "Couldn't get region %x[%x]",
			(int)aux_start, (int)aux_len);
		goto free_region1;
	}

	ai->pcimem = ioremap(mem_start, mem_len);
	if (!ai->pcimem) {
		airo_print_err("", "Couldn't map region %x[%x]",
			(int)mem_start, (int)mem_len);
		goto free_region2;
	}
	ai->pciaux = ioremap(aux_start, aux_len);
	if (!ai->pciaux) {
		airo_print_err("", "Couldn't map region %x[%x]",
			(int)aux_start, (int)aux_len);
		goto free_memmap;
	}

	 
	ai->shared = pci_alloc_consistent(pci, PCI_SHARED_LEN, &ai->shared_dma);
	if (!ai->shared) {
		airo_print_err("", "Couldn't alloc_consistent %d",
			PCI_SHARED_LEN);
		goto free_auxmap;
	}

	 
	busaddroff = ai->shared_dma;
	pciaddroff = ai->pciaux + AUX_OFFSET;
	vpackoff   = ai->shared;

	 
	for(i = 0; i < MPI_MAX_FIDS; i++) {
		ai->rxfids[i].pending = 0;
		ai->rxfids[i].card_ram_off = pciaddroff;
		ai->rxfids[i].virtual_host_addr = vpackoff;
		ai->rxfids[i].rx_desc.host_addr = busaddroff;
		ai->rxfids[i].rx_desc.valid = 1;
		ai->rxfids[i].rx_desc.len = PKTSIZE;
		ai->rxfids[i].rx_desc.rdy = 0;

		pciaddroff += sizeof(RxFid);
		busaddroff += PKTSIZE;
		vpackoff   += PKTSIZE;
	}

	 
	for(i = 0; i < MPI_MAX_FIDS; i++) {
		ai->txfids[i].card_ram_off = pciaddroff;
		ai->txfids[i].virtual_host_addr = vpackoff;
		ai->txfids[i].tx_desc.valid = 1;
		ai->txfids[i].tx_desc.host_addr = busaddroff;
		memcpy(ai->txfids[i].virtual_host_addr,
			&wifictlhdr8023, sizeof(wifictlhdr8023));

		pciaddroff += sizeof(TxFid);
		busaddroff += PKTSIZE;
		vpackoff   += PKTSIZE;
	}
	ai->txfids[i-1].tx_desc.eoc = 1;  

	 
	ai->config_desc.card_ram_off = pciaddroff;
	ai->config_desc.virtual_host_addr = vpackoff;
	ai->config_desc.rid_desc.host_addr = busaddroff;
	ai->ridbus = busaddroff;
	ai->config_desc.rid_desc.rid = 0;
	ai->config_desc.rid_desc.len = RIDSIZE;
	ai->config_desc.rid_desc.valid = 1;
	pciaddroff += sizeof(Rid);
	busaddroff += RIDSIZE;
	vpackoff   += RIDSIZE;

	 
	if (mpi_init_descriptors (ai) != SUCCESS)
		goto free_shared;

	return 0;
 free_shared:
	pci_free_consistent(pci, PCI_SHARED_LEN, ai->shared, ai->shared_dma);
 free_auxmap:
	iounmap(ai->pciaux);
 free_memmap:
	iounmap(ai->pcimem);
 free_region2:
	release_mem_region(aux_start, aux_len);
 free_region1:
	release_mem_region(mem_start, mem_len);
 out:
	return rc;
}