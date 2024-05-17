static void __ip_vs_unlink_dest(struct ip_vs_service *svc,
				struct ip_vs_dest *dest,
				int svcupd)
{
	dest->flags &= ~IP_VS_DEST_F_AVAILABLE;

	 
	list_del(&dest->n_list);
	svc->num_dests--;

	 
	if (svcupd && svc->scheduler->update_service)
			svc->scheduler->update_service(svc);
}
