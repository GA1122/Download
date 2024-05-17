static __init int rb_hammer_test(void *arg)
{
	while (!kthread_should_stop()) {

		 
		smp_call_function(rb_ipi, NULL, 1);
		 
		schedule();
	}

	return 0;
}
