static void need_waiter(struct ipmi_smi *intf)
{
	 
	if (!timer_pending(&ipmi_timer))
		mod_timer(&ipmi_timer, jiffies + IPMI_TIMEOUT_JIFFIES);
}
