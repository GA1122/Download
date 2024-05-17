config_sim(
	config_tree *ptree
	)
{
	int i;
	server_info *serv_info;
	attr_val *init_stmt;
	sim_node *sim_n;

	 
	sim_n = HEAD_PFIFO(ptree->sim_details);
	if (NULL == sim_n) {
		fprintf(stderr, "ERROR!! I couldn't find a \"simulate\" block for configuring the simulator.\n");
		fprintf(stderr, "\tCheck your configuration file.\n");
		exit(1);
	}

	 
	init_stmt = HEAD_PFIFO(sim_n->init_opts);
	for (; init_stmt != NULL; init_stmt = init_stmt->link) {
		switch(init_stmt->attr) {

		case T_Beep_Delay:
			simulation.beep_delay = init_stmt->value.d;
			break;

		case T_Sim_Duration:
			simulation.end_time = init_stmt->value.d;
			break;

		default:
			fprintf(stderr,
				"Unknown simulator init token %d\n",
				init_stmt->attr);
			exit(1);
		}
	}

	 
	simulation.num_of_servers = 0;
	serv_info = HEAD_PFIFO(sim_n->servers);
	for (; serv_info != NULL; serv_info = serv_info->link)
		simulation.num_of_servers++;
	simulation.servers = emalloc(simulation.num_of_servers *
				     sizeof(simulation.servers[0]));

	i = 0;
	serv_info = HEAD_PFIFO(sim_n->servers);
	for (; serv_info != NULL; serv_info = serv_info->link) {
		if (NULL == serv_info) {
			fprintf(stderr, "Simulator server list is corrupt\n");
			exit(1);
		} else {
			simulation.servers[i] = *serv_info;
			simulation.servers[i].link = NULL;
			i++;
		}
	}

	printf("Creating server associations\n");
	create_server_associations();
	fprintf(stderr,"\tServer associations successfully created!!\n");
}