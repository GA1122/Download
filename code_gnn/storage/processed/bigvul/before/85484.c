int sas_register_ports(struct sas_ha_struct *sas_ha)
{
	int i;

	 
	for (i = 0; i < sas_ha->num_phys; i++) {
		struct asd_sas_port *port = sas_ha->sas_port[i];

		sas_init_port(port, sas_ha, i);
		sas_init_disc(&port->disc, port);
	}
	return 0;
}