static void rds_ib_unregister_client(void)
{
	ib_unregister_client(&rds_ib_client);
	 
	flush_workqueue(rds_wq);
}
