_epilog_complete(uint32_t jobid, int rc)
{
	int ret = SLURM_SUCCESS;

	if (conf->msg_aggr_window_msgs > 1) {
		 
		slurm_msg_t *msg = xmalloc(sizeof(slurm_msg_t));
		epilog_complete_msg_t *req =
			xmalloc(sizeof(epilog_complete_msg_t));

		_epilog_complete_msg_setup(msg, req, jobid, rc);

		 
		req->node_name   = xstrdup(conf->node_name);

		msg_aggr_add_msg(msg, 0, NULL);
	} else {
		slurm_msg_t msg;
		epilog_complete_msg_t req;

		_epilog_complete_msg_setup(&msg, &req, jobid, rc);

		 
		if (slurm_send_only_controller_msg(&msg) < 0) {
			error("Unable to send epilog complete message: %m");
			ret = SLURM_ERROR;
		} else {
			debug("Job %u: sent epilog complete msg: rc = %d",
			      jobid, rc);
		}
	}
	return ret;
}
