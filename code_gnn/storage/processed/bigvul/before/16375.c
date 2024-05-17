pseudo_register_mpi_master_info( ClassAd* ad ) 
{
	char *addr = NULL;

	if( ! ad->LookupString(ATTR_MPI_MASTER_ADDR, &addr) ) {
		dprintf( D_ALWAYS,
				 "ERROR: mpi_master_info ClassAd doesn't contain %s\n",
				 ATTR_MPI_MASTER_ADDR );
		return -1;
	}
	if( ! Shadow->setMpiMasterInfo(addr) ) {
		dprintf( D_ALWAYS, "ERROR: recieved "
				 "pseudo_register_mpi_master_info for a non-MPI job!\n" );
		return -1;
	}
	return 0;
}
