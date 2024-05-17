process_method_call(dbus_queue_ent_t *ent)
{
	ssize_t ret;

	if (!ent)
		return NULL;

	 
	ent_ptr = ent;
	if (write(dbus_in_pipe[1], ent, 1) != 1)
		log_message(LOG_INFO, "Write from DBus thread to main thread failed");

	 
	while ((ret = read(dbus_out_pipe[0], ent, 1)) == -1 && errno == EINTR) {
		log_message(LOG_INFO, "dbus_out_pipe read returned EINTR");
	}
	if (ret == -1)
		log_message(LOG_INFO, "DBus response read error - errno = %d", errno);

#ifdef DBUS_DEBUG
	if (ent->reply != DBUS_SUCCESS) {
		char *iname;

		if (ent->reply == DBUS_INTERFACE_NOT_FOUND)
			log_message(LOG_INFO, "Unable to find DBus requested instance %s/%d/%s", ent->ifname, ent->vrid, family_str(ent->family));
		else if (ent->reply == DBUS_OBJECT_ALREADY_EXISTS)
			log_message(LOG_INFO, "Unable to create DBus requested object with instance %s/%d/%s", ent->ifname, ent->vrid, family_str(ent->family));
		else if (ent->reply == DBUS_INSTANCE_NOT_FOUND) {
			g_variant_get(ent->args, "(s)", &iname);
			log_message(LOG_INFO, "Unable to find DBus requested instance %s", iname);
		}
		else
			log_message(LOG_INFO, "Unknown DBus reply %d", ent->reply);
	}
#endif

	return ent;
}