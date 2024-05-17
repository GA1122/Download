const char *v4l2_norm_to_name(v4l2_std_id id)
{
	u32 myid = id;
	int i;

	 
	BUG_ON(myid != id);

	for (i = 0; standards[i].std; i++)
		if (myid == standards[i].std)
			break;
	return standards[i].descr;
}
