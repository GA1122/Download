static int pcd_ready_wait(struct pcd_unit *cd, int tmo)
{
	char tr_cmd[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int k, p;

	k = 0;
	while (k < tmo) {
		cd->last_sense = 0;
		pcd_atapi(cd, tr_cmd, 0, NULL, DBMSG("test unit ready"));
		p = cd->last_sense;
		if (!p)
			return 0;
		if (!(((p & 0xffff) == 0x0402) || ((p & 0xff) == 6)))
			return p;
		k++;
		pcd_sleep(HZ);
	}
	return 0x000020;	 
}