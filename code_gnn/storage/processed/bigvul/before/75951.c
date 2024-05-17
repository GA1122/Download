dbus_reload(list o, list n)
{
	element e1, e2, e3;
	vrrp_t *vrrp_n, *vrrp_o, *vrrp_n3;

	if (!LIST_ISEMPTY(n)) {
		for (e1 = LIST_HEAD(n); e1; ELEMENT_NEXT(e1)) {
			char *n_name;
			bool match_found;

			vrrp_n = ELEMENT_DATA(e1);

			if (LIST_ISEMPTY(o)) {
				dbus_create_object(vrrp_n);
				continue;
			}

			n_name = IF_BASE_IFP(vrrp_n->ifp)->ifname;

			 
			for (e2 = LIST_HEAD(o), match_found = false; e2 && !match_found; ELEMENT_NEXT(e2)) {
				vrrp_o = ELEMENT_DATA(e2);

				if (vrrp_n->vrid == vrrp_o->vrid &&
				    vrrp_n->family == vrrp_o->family &&
				    !strcmp(n_name, IF_BASE_IFP(vrrp_o->ifp)->ifname)) {
					 
					if (!strcmp(vrrp_n->iname, vrrp_o->iname)) {
						match_found = true;
						break;
					}

					 
					for (e3 = LIST_HEAD(n); e3; ELEMENT_NEXT(e3)) {
						vrrp_n3 = ELEMENT_DATA(e3);
						if (!strcmp(vrrp_o->iname, vrrp_n3->iname)) {
							match_found = true;
							break;
						}
					}
				}
			}

			if (match_found)
				continue;

			dbus_create_object(vrrp_n);
		}
	}

	 
	dbus_send_reload_signal();

	 
	thread_add_read(master, handle_dbus_msg, NULL, dbus_in_pipe[0], TIMER_NEVER);
}
