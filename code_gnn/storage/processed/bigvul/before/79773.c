static int uvesafb_exec(struct uvesafb_ktask *task)
{
	static int seq;
	struct cn_msg *m;
	int err;
	int len = sizeof(task->t) + task->t.buf_len;

	 
	if (sizeof(*m) + len > CONNECTOR_MAX_MSG_SIZE) {
		pr_warn("message too long (%d), can't execute task\n",
			(int)(sizeof(*m) + len));
		return -E2BIG;
	}

	m = kzalloc(sizeof(*m) + len, GFP_KERNEL);
	if (!m)
		return -ENOMEM;

	init_completion(task->done);

	memcpy(&m->id, &uvesafb_cn_id, sizeof(m->id));
	m->seq = seq;
	m->len = len;
	m->ack = prandom_u32();

	 
	memcpy(m + 1, &task->t, sizeof(task->t));

	 
	memcpy((u8 *)(m + 1) + sizeof(task->t), task->buf, task->t.buf_len);

	 
	task->ack = m->ack;

	mutex_lock(&uvfb_lock);

	 
	if (uvfb_tasks[seq]) {
		mutex_unlock(&uvfb_lock);
		err = -EBUSY;
		goto out;
	}

	 
	uvfb_tasks[seq] = task;
	mutex_unlock(&uvfb_lock);

	err = cn_netlink_send(m, 0, 0, GFP_KERNEL);
	if (err == -ESRCH) {
		 
		err = uvesafb_helper_start();
		if (err) {
			pr_err("failed to execute %s\n", v86d_path);
			pr_err("make sure that the v86d helper is installed and executable\n");
		} else {
			v86d_started = 1;
			err = cn_netlink_send(m, 0, 0, gfp_any());
			if (err == -ENOBUFS)
				err = 0;
		}
	} else if (err == -ENOBUFS)
		err = 0;

	if (!err && !(task->t.flags & TF_EXIT))
		err = !wait_for_completion_timeout(task->done,
				msecs_to_jiffies(UVESAFB_TIMEOUT));

	mutex_lock(&uvfb_lock);
	uvfb_tasks[seq] = NULL;
	mutex_unlock(&uvfb_lock);

	seq++;
	if (seq >= UVESAFB_TASKS_MAX)
		seq = 0;
out:
	kfree(m);
	return err;
}
