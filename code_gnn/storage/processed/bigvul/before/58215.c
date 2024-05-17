asmlinkage void __sched schedule_user(void)
{
	 
	user_exit();
	schedule();
	user_enter();
}
