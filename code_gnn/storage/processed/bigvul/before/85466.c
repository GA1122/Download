static void sas_chain_work(struct sas_ha_struct *ha, struct sas_work *sw)
{
	 
	queue_work(ha->disco_q, &sw->work);
}
