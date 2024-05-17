static int snd_timer_user_gstatus(struct file *file,
				  struct snd_timer_gstatus __user *_gstatus)
{
	struct snd_timer_gstatus gstatus;
	struct snd_timer_id tid;
	struct snd_timer *t;
	int err = 0;

	if (copy_from_user(&gstatus, _gstatus, sizeof(gstatus)))
		return -EFAULT;
	tid = gstatus.tid;
	memset(&gstatus, 0, sizeof(gstatus));
	gstatus.tid = tid;
	mutex_lock(&register_mutex);
	t = snd_timer_find(&tid);
	if (t != NULL) {
		if (t->hw.c_resolution)
			gstatus.resolution = t->hw.c_resolution(t);
		else
			gstatus.resolution = t->hw.resolution;
		if (t->hw.precise_resolution) {
			t->hw.precise_resolution(t, &gstatus.resolution_num,
						 &gstatus.resolution_den);
		} else {
			gstatus.resolution_num = gstatus.resolution;
			gstatus.resolution_den = 1000000000uL;
		}
	} else {
		err = -ENODEV;
	}
	mutex_unlock(&register_mutex);
	if (err >= 0 && copy_to_user(_gstatus, &gstatus, sizeof(gstatus)))
		err = -EFAULT;
	return err;
}