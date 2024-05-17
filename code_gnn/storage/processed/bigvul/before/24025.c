static void enable_interrupts( struct airo_info *ai ) {
	 
	OUT4500( ai, EVINTEN, STATUS_INTS );
}
