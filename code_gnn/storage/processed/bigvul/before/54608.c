static int __init snd_hrtimer_init(void)
{
	struct snd_timer *timer;
	int err;

	resolution = hrtimer_resolution;

	 
	err = snd_timer_global_new("hrtimer", SNDRV_TIMER_GLOBAL_HRTIMER,
				   &timer);
	if (err < 0)
		return err;

	timer->module = THIS_MODULE;
	strcpy(timer->name, "HR timer");
	timer->hw = hrtimer_hw;
	timer->hw.resolution = resolution;
	timer->hw.ticks = NANO_SEC / resolution;

	err = snd_timer_global_register(timer);
	if (err < 0) {
		snd_timer_global_free(timer);
		return err;
	}
	mytimer = timer;  

	return 0;
}
