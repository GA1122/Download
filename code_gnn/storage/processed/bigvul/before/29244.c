static void ip_vs_copy_usvc_compat(struct ip_vs_service_user_kern *usvc,
				  struct ip_vs_service_user *usvc_compat)
{
	usvc->af		= AF_INET;
	usvc->protocol		= usvc_compat->protocol;
	usvc->addr.ip		= usvc_compat->addr;
	usvc->port		= usvc_compat->port;
	usvc->fwmark		= usvc_compat->fwmark;

	 
	usvc->sched_name	= usvc_compat->sched_name;

	usvc->flags		= usvc_compat->flags;
	usvc->timeout		= usvc_compat->timeout;
	usvc->netmask		= usvc_compat->netmask;
}