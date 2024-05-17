copy_set_server_options(ServerOptions *dst, ServerOptions *src, int preauth)
{
#define M_CP_INTOPT(n) do {\
	if (src->n != -1) \
		dst->n = src->n; \
} while (0)

	M_CP_INTOPT(password_authentication);
	M_CP_INTOPT(gss_authentication);
	M_CP_INTOPT(pubkey_authentication);
	M_CP_INTOPT(kerberos_authentication);
	M_CP_INTOPT(hostbased_authentication);
	M_CP_INTOPT(hostbased_uses_name_from_packet_only);
	M_CP_INTOPT(kbd_interactive_authentication);
	M_CP_INTOPT(permit_root_login);
	M_CP_INTOPT(permit_empty_passwd);

	M_CP_INTOPT(allow_tcp_forwarding);
	M_CP_INTOPT(allow_streamlocal_forwarding);
	M_CP_INTOPT(allow_agent_forwarding);
	M_CP_INTOPT(permit_tun);
	M_CP_INTOPT(fwd_opts.gateway_ports);
	M_CP_INTOPT(fwd_opts.streamlocal_bind_unlink);
	M_CP_INTOPT(x11_display_offset);
	M_CP_INTOPT(x11_forwarding);
	M_CP_INTOPT(x11_use_localhost);
	M_CP_INTOPT(permit_tty);
	M_CP_INTOPT(permit_user_rc);
	M_CP_INTOPT(max_sessions);
	M_CP_INTOPT(max_authtries);
	M_CP_INTOPT(ip_qos_interactive);
	M_CP_INTOPT(ip_qos_bulk);
	M_CP_INTOPT(rekey_limit);
	M_CP_INTOPT(rekey_interval);

	 
	if (src->fwd_opts.streamlocal_bind_mask != (mode_t)-1) {
		dst->fwd_opts.streamlocal_bind_mask =
		    src->fwd_opts.streamlocal_bind_mask;
	}

	 
#define M_CP_STROPT(n) do {\
	if (src->n != NULL && dst->n != src->n) { \
		free(dst->n); \
		dst->n = src->n; \
	} \
} while(0)
#define M_CP_STRARRAYOPT(n, num_n) do {\
	if (src->num_n != 0) { \
		for (dst->num_n = 0; dst->num_n < src->num_n; dst->num_n++) \
			dst->n[dst->num_n] = xstrdup(src->n[dst->num_n]); \
	} \
} while(0)

	 
	COPY_MATCH_STRING_OPTS();

	 
	assemble_algorithms(dst);

	 
	if (preauth)
		return;

	 
	M_CP_STROPT(adm_forced_command);
	if (option_clear_or_none(dst->adm_forced_command)) {
		free(dst->adm_forced_command);
		dst->adm_forced_command = NULL;
	}
	M_CP_STROPT(chroot_directory);
	if (option_clear_or_none(dst->chroot_directory)) {
		free(dst->chroot_directory);
		dst->chroot_directory = NULL;
	}
}
