static void net_rps_action_and_irq_enable(struct softnet_data *sd)
{
#ifdef CONFIG_RPS
	struct softnet_data *remsd = sd->rps_ipi_list;

	if (remsd) {
		sd->rps_ipi_list = NULL;

		local_irq_enable();

		 
		while (remsd) {
			struct softnet_data *next = remsd->rps_ipi_next;

			if (cpu_online(remsd->cpu))
				smp_call_function_single_async(remsd->cpu,
							   &remsd->csd);
			remsd = next;
		}
	} else
#endif
		local_irq_enable();
}