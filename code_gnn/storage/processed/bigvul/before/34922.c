rpc_task_force_reencode(struct rpc_task *task)
{
	task->tk_rqstp->rq_snd_buf.len = 0;
	task->tk_rqstp->rq_bytes_sent = 0;
}