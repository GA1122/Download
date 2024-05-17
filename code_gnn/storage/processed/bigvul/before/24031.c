static int flashputbuf(struct airo_info *ai){
	int            nwords;

	 
	if (test_bit(FLAG_MPI,&ai->flags))
		memcpy_toio(ai->pciaux + 0x8000, ai->flash, FLASHSIZE);
	else {
		OUT4500(ai,AUXPAGE,0x100);
		OUT4500(ai,AUXOFF,0);

		for(nwords=0;nwords != FLASHSIZE / 2;nwords++){
			OUT4500(ai,AUXDATA,ai->flash[nwords] & 0xffff);
		}
	}
	OUT4500(ai,SWS0,0x8000);

	return 0;
}