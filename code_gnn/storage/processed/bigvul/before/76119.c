child_killed_thread(thread_t *thread)
{
	thread_master_t *m = thread->master;

	 
	if (thread->type == THREAD_CHILD_TIMEOUT)
		kill(-getpgid(thread->u.c.pid), SIGKILL);

	 
	if (!&m->child.rb_root.rb_node && !m->shutdown_timer_running)
		thread_add_terminate_event(m);

	return 0;
}