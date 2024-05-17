static void lockd_unregister_notifiers(void)
{
	unregister_inetaddr_notifier(&lockd_inetaddr_notifier);
#if IS_ENABLED(CONFIG_IPV6)
	unregister_inet6addr_notifier(&lockd_inet6addr_notifier);
#endif
}
