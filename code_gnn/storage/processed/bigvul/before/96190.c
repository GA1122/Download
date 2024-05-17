int mgr_log_mask(p_fm_config_conx_hdlt hdl, fm_mgr_type_t mgr, int argc, char *argv[]) {
	fm_mgr_config_errno_t	res;
	fm_msg_ret_code_t		ret_code;
	uint32_t				mask=0;
	char buf[32];			 

	if (mgr == FM_MGR_PM) {
		fprintf(stderr, "pmLogMask:\n");
		fprintf(stderr, "\tThis command is not supported any more.  The logging of the\n");
		fprintf(stderr, "\tPerformance Manager(PM) is now\n");
		fprintf(stderr, "\tbased on the logging of the Subnet manager(SM).  Use the\n");
		fprintf(stderr, "\tsmLogMask command for changing the logging level of the\n");
		fprintf(stderr, "\tSM and PM\n");

	} else if (argc == 2) {
		mask = strtoul(argv[1], NULL, 0);
		memcpy(buf, &mask, sizeof(mask));
		strncpy(buf+sizeof(mask), argv[0], sizeof(buf)-sizeof(mask));
		buf[sizeof(buf)-1] = '\0';
		if((res = fm_mgr_simple_query(hdl, FM_ACT_GET, FM_DT_LOG_MASK, mgr, sizeof(buf), (void *)&buf[0], &ret_code)) != FM_CONF_OK)
		{
			fprintf(stderr, "mgr_log_mask: Failed to retrieve data: \n"
				   "\tError:(%d) %s \n\tRet code:(%d) %s\n",
				   res, fm_mgr_get_error_str(res),ret_code,
				   fm_mgr_get_resp_error_str(ret_code));
		} else {
			printf("mgr_log_mask: Successfully sent Log Mask control to local mgr instance\n");
		}
	} else {
		fprintf(stderr, "mgr_log_mask: must specify the subsystem and mask\n");
	}

	return 0;
}