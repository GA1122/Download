handle_dbus_msg(__attribute__((unused)) thread_t *thread)
{
	dbus_queue_ent_t *ent;
	char recv_buf;
	vrrp_t *vrrp;
#ifdef _WITH_DBUS_CREATE_INSTANCE_
	gchar *name;
#endif

	if (read(dbus_in_pipe[0], &recv_buf, 1) != 1)
		log_message(LOG_INFO, "Read from DBus thread to vrrp thread failed");

	if ((ent = ent_ptr) != NULL) {
		ent->reply = DBUS_SUCCESS;

		if (ent->action == DBUS_PRINT_DATA) {
			log_message(LOG_INFO, "Printing VRRP data on DBus request");
			vrrp_print_data();

		}
		else if (ent->action == DBUS_PRINT_STATS) {
			log_message(LOG_INFO, "Printing VRRP stats on DBus request");
			vrrp_print_stats();
		}
#ifdef _WITH_DBUS_CREATE_INSTANCE_
		else if (ent->action == DBUS_CREATE_INSTANCE) {
			g_variant_get(ent->args, "(s)", &name);
			ent->reply = dbus_create_object_params(name, ent->ifname, ent->vrid, ent->family, true);
		}
		else if (ent->action == DBUS_DESTROY_INSTANCE) {
			g_variant_get(ent->args, "(s)", &name);
			if (!dbus_unregister_object(name))
				ent->reply = DBUS_INSTANCE_NOT_FOUND;
		}
#endif
		else if (ent->action == DBUS_SEND_GARP) {
			ent->reply = DBUS_INTERFACE_NOT_FOUND;
			vrrp = get_vrrp_instance(ent->ifname, ent->vrid, ent->family);
			if (vrrp) {
				log_message(LOG_INFO, "Sending garps on %s on DBus request", vrrp->iname);
				vrrp_send_link_update(vrrp, 1);
				ent->reply = DBUS_SUCCESS;
			}
		}
		else if (ent->action == DBUS_GET_NAME ||
			 ent->action == DBUS_GET_STATUS) {
			 
			ent->reply = DBUS_INTERFACE_NOT_FOUND;

			vrrp = get_vrrp_instance(ent->ifname, ent->vrid, ent->family);

			if (vrrp) {
				 
				if (ent->action == DBUS_GET_NAME)
					ent->args = g_variant_new("(s)", vrrp->iname);
				else if (ent->action == DBUS_GET_STATUS)
					ent->args = g_variant_new("(us)", vrrp->state, state_str(vrrp->state));
				else
					ent->args = NULL;	  
				ent->reply = DBUS_SUCCESS;
			}
		}
		if (write(dbus_out_pipe[1], ent, 1) != 1)
			log_message(LOG_INFO, "Write from main thread to DBus thread failed");
	}

	thread_add_read(master, handle_dbus_msg, NULL, dbus_in_pipe[0], TIMER_NEVER);

	return 0;
}