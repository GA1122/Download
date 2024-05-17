handle_method_call(__attribute__((unused)) GDBusConnection *connection,
		   __attribute__((unused)) const gchar	   *sender,
					   const gchar	   *object_path,
					   const gchar	   *interface_name,
					   const gchar	   *method_name,
#ifndef _WITH_DBUS_CREATE_INSTANCE_
		   __attribute__((unused))
#endif
					   GVariant *parameters,
		   GDBusMethodInvocation *invocation,
		   __attribute__((unused)) gpointer user_data)
{
#ifdef _WITH_DBUS_CREATE_INSTANCE_
	char *iname;
	char *ifname;
	size_t len;
	unsigned family;
#endif
	dbus_queue_ent_t ent;
	char ifname_str[sizeof ((vrrp_t*)NULL)->ifp->ifname];

	if (!g_strcmp0(interface_name, DBUS_VRRP_INTERFACE)) {
		if (!g_strcmp0(method_name, "PrintData")) {
			ent.action = DBUS_PRINT_DATA;
			process_method_call(&ent);
			g_dbus_method_invocation_return_value(invocation, NULL);
		}
		else if (g_strcmp0(method_name, "PrintStats") == 0) {
			ent.action = DBUS_PRINT_STATS;
			process_method_call(&ent);
			g_dbus_method_invocation_return_value(invocation, NULL);
		}
		else if (g_strcmp0(method_name, "ReloadConfig") == 0) {
			g_dbus_method_invocation_return_value(invocation, NULL);
			kill(getppid(), SIGHUP);
		}
#ifdef _WITH_DBUS_CREATE_INSTANCE_
		else if (g_strcmp0(method_name, "CreateInstance") == 0) {
			g_variant_get(parameters, "(ssuu)", &iname, &ifname, &ent.vrid, &family);
			len = strlen(ifname);
			if (len == 0 || len >= IFNAMSIZ) {
				log_message(LOG_INFO, "Interface name '%s' too long for CreateInstance", ifname);
				g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR, G_DBUS_ERROR_INVALID_ARGS, "Interface name empty or too long");
				return;
			}
			ent.action = DBUS_CREATE_INSTANCE;
			ent.ifname = ifname;
			ent.family = family == 4 ? AF_INET : family == 6 ? AF_INET6 : AF_UNSPEC;
			ent.args = g_variant_new("(s)", iname);
			process_method_call(&ent);
			g_variant_unref(ent.args);
			g_dbus_method_invocation_return_value(invocation, NULL);
		}
		else if (g_strcmp0(method_name, "DestroyInstance") == 0) {
			ent.action = DBUS_DESTROY_INSTANCE;
			ent.args = parameters;
			process_method_call(&ent);

			if (ent.reply == DBUS_INSTANCE_NOT_FOUND) {
				g_variant_get(parameters, "(s)", &iname);
				g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR, G_DBUS_ERROR_INVALID_ARGS, "Instance '%s' not found", iname);
			}
			else
				g_dbus_method_invocation_return_value(invocation, NULL);
		}
#endif
		else {
			log_message(LOG_INFO, "Method %s has not been implemented yet", method_name);
			g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR, G_DBUS_ERROR_MATCH_RULE_NOT_FOUND, "Method not implemented");
		}

		return;
	}

	if (!g_strcmp0(interface_name, DBUS_VRRP_INSTANCE_INTERFACE)) {
		if (!g_strcmp0(method_name, "SendGarp")) {
			get_interface_ids(object_path, ifname_str, &ent.vrid, &ent.family);
			ent.action = DBUS_SEND_GARP;
			ent.ifname = ifname_str;
			process_method_call(&ent);
			if (ent.reply ==  DBUS_INTERFACE_NOT_FOUND)
				g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR, G_DBUS_ERROR_INVALID_ARGS, "Instance '%s/%d/%s' not found", ifname_str, ent.vrid, family_str(ent.family));
			else
				g_dbus_method_invocation_return_value(invocation, NULL);
		} else {
			log_message(LOG_INFO, "Method %s has not been implemented yet", method_name);
			g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR, G_DBUS_ERROR_MATCH_RULE_NOT_FOUND, "Method not implemented");
		}

		return;
	}

	log_message(LOG_INFO, "Interface %s has not been implemented yet", interface_name);
	g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR, G_DBUS_ERROR_MATCH_RULE_NOT_FOUND, "Interface not implemented");
}