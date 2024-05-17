static void lockd_svc_exit_thread(void)
{
	lockd_unregister_notifiers();
	svc_exit_thread(nlmsvc_rqst);
}
