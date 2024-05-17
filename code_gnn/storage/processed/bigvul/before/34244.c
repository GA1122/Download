proc_do_sync_threshold(ctl_table *table, int write,
		       void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int *valp = table->data;
	int val[2];
	int rc;

	 
	memcpy(val, valp, sizeof(val));

	rc = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write && (valp[0] < 0 || valp[1] < 0 ||
	    (valp[0] >= valp[1] && valp[1]))) {
		 
		memcpy(valp, val, sizeof(val));
	}
	return rc;
}
