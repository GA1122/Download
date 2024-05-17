static __init int rb_test(void *arg)
{
	struct rb_test_data *data = arg;

	while (!kthread_should_stop()) {
		rb_write_something(data, false);
		data->cnt++;

		set_current_state(TASK_INTERRUPTIBLE);
		 
		usleep_range(((data->cnt % 3) + 1) * 100, 1000);
	}

	return 0;
}
