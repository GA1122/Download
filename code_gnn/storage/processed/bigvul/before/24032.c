static int flashrestart(struct airo_info *ai,struct net_device *dev){
	int    i,status;

	ssleep(1);			 
	clear_bit (FLAG_FLASHING, &ai->flags);
	if (test_bit(FLAG_MPI, &ai->flags)) {
		status = mpi_init_descriptors(ai);
		if (status != SUCCESS)
			return status;
	}
	status = setup_card(ai, dev->dev_addr, 1);

	if (!test_bit(FLAG_MPI,&ai->flags))
		for( i = 0; i < MAX_FIDS; i++ ) {
			ai->fids[i] = transmit_allocate
				( ai, AIRO_DEF_MTU, i >= MAX_FIDS / 2 );
		}

	ssleep(1);			 
	return status;
}