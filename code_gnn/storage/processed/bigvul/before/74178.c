dump_config_tree(
	config_tree *ptree,
	FILE *df,
	int comment
	)
{
	peer_node *peern;
	unpeer_node *unpeern;
	attr_val *atrv;
	address_node *addr;
	address_node *peer_addr;
	address_node *fudge_addr;
	filegen_node *fgen_node;
	restrict_node *rest_node;
	addr_opts_node *addr_opts;
	setvar_node *setv_node;
	nic_rule_node *rule_node;
	int_node *i_n;
	int_node *flags;
	string_node *str_node;

	const char *s;
	char *s1;
	char *s2;
	char timestamp[80];
	int enable;

	DPRINTF(1, ("dump_config_tree(%p)\n", ptree));

	if (comment) {
		if (!strftime(timestamp, sizeof(timestamp),
			      "%Y-%m-%d %H:%M:%S",
			      localtime(&ptree->timestamp)))
			timestamp[0] = '\0';

		fprintf(df, "# %s %s %s\n",
			timestamp,
			(CONF_SOURCE_NTPQ == ptree->source.attr)
			    ? "ntpq remote config from"
			    : "startup configuration file",
			ptree->source.value.s);
	}

	 
	atrv = HEAD_PFIFO(ptree->vars);
	for ( ; atrv != NULL; atrv = atrv->link) {
		switch (atrv->type) {
#ifdef DEBUG
		default:
			fprintf(df, "\n# dump error:\n"
				"# unknown vars type %d (%s) for %s\n",
				atrv->type, token_name(atrv->type),
				token_name(atrv->attr));
			break;
#endif
		case T_Double:
			fprintf(df, "%s %s\n", keyword(atrv->attr),
				normal_dtoa(atrv->value.d));
			break;
			
		case T_Integer:
			fprintf(df, "%s %d\n", keyword(atrv->attr),
				atrv->value.i);
			break;

		case T_String:
			fprintf(df, "%s \"%s\"", keyword(atrv->attr),
				atrv->value.s);
			if (T_Driftfile == atrv->attr &&
			    atrv->link != NULL &&
			    T_WanderThreshold == atrv->link->attr) {
				atrv = atrv->link;
				fprintf(df, " %s\n",
					normal_dtoa(atrv->value.d));
			} else {
				fprintf(df, "\n");
			}
			break;
		}
	}

	atrv = HEAD_PFIFO(ptree->logconfig);
	if (atrv != NULL) {
		fprintf(df, "logconfig");
		for ( ; atrv != NULL; atrv = atrv->link)
			fprintf(df, " %c%s", atrv->attr, atrv->value.s);
		fprintf(df, "\n");
	}

	if (ptree->stats_dir)
		fprintf(df, "statsdir \"%s\"\n", ptree->stats_dir);

	i_n = HEAD_PFIFO(ptree->stats_list);
	if (i_n != NULL) {
		fprintf(df, "statistics");
		for ( ; i_n != NULL; i_n = i_n->link)
			fprintf(df, " %s", keyword(i_n->i));	
		fprintf(df, "\n");
	}

	fgen_node = HEAD_PFIFO(ptree->filegen_opts);
	for ( ; fgen_node != NULL; fgen_node = fgen_node->link) {
		atrv = HEAD_PFIFO(fgen_node->options);
		if (atrv != NULL) {
			fprintf(df, "filegen %s", 
				keyword(fgen_node->filegen_token));
			for ( ; atrv != NULL; atrv = atrv->link) {
				switch (atrv->attr) {
#ifdef DEBUG
				default:
					fprintf(df, "\n# dump error:\n"
						"# unknown filegen option token %s\n"
						"filegen %s",
						token_name(atrv->attr),
						keyword(fgen_node->filegen_token));
					break;
#endif
				case T_File:
					fprintf(df, " file %s",
						atrv->value.s);
					break;

				case T_Type:
					fprintf(df, " type %s",
						keyword(atrv->value.i));
					break;

				case T_Flag:
					fprintf(df, " %s",
						keyword(atrv->value.i));
					break;
				}
			}
			fprintf(df, "\n");
		}
	}

	atrv = HEAD_PFIFO(ptree->auth.crypto_cmd_list);
	if (atrv != NULL) {
		fprintf(df, "crypto");
		for ( ; atrv != NULL; atrv = atrv->link) {
			fprintf(df, " %s %s", keyword(atrv->attr),
				atrv->value.s);
		}
		fprintf(df, "\n");
	}

	if (ptree->auth.revoke != 0)
		fprintf(df, "revoke %d\n", ptree->auth.revoke);

	if (ptree->auth.keysdir != NULL)
		fprintf(df, "keysdir \"%s\"\n", ptree->auth.keysdir);

	if (ptree->auth.keys != NULL)
		fprintf(df, "keys \"%s\"\n", ptree->auth.keys);

	atrv = HEAD_PFIFO(ptree->auth.trusted_key_list);
	if (atrv != NULL) {
		fprintf(df, "trustedkey");
		for ( ; atrv != NULL; atrv = atrv->link) {
			if (T_Integer == atrv->type)
				fprintf(df, " %d", atrv->value.i);
			else if (T_Intrange == atrv->type)
				fprintf(df, " (%d ... %d)",
					atrv->value.r.first,
					atrv->value.r.last);
#ifdef DEBUG
			else
				fprintf(df, "\n# dump error:\n"
					"# unknown trustedkey attr type %d\n"
					"trustedkey", atrv->type);
#endif
		}
		fprintf(df, "\n");
	}

	if (ptree->auth.control_key)
		fprintf(df, "controlkey %d\n", ptree->auth.control_key);

	if (ptree->auth.request_key)
		fprintf(df, "requestkey %d\n", ptree->auth.request_key);

	 
	for (enable = 1; enable >= 0; enable--) {
		atrv = (enable)
			   ? HEAD_PFIFO(ptree->enable_opts)
			   : HEAD_PFIFO(ptree->disable_opts);
		if (atrv != NULL) {
			fprintf(df, (enable)
					? "enable"
					: "disable");
			for ( ; atrv != NULL; atrv = atrv->link)
				fprintf(df, " %s",
					keyword(atrv->value.i));
			fprintf(df, "\n");
		}
	}

	atrv = HEAD_PFIFO(ptree->orphan_cmds);
	if (atrv != NULL) {
		fprintf(df, "tos");
		for ( ; atrv != NULL; atrv = atrv->link) {
			switch (atrv->type) {
#ifdef DEBUG
			default:
				fprintf(df, "\n# dump error:\n"
					"# unknown tos attr type %d %s\n"
					"tos", atrv->type, 
					token_name(atrv->type));
				break;
#endif
			case T_Double:
				fprintf(df, " %s %s", 
					keyword(atrv->attr),
					normal_dtoa(atrv->value.d));
				break;
			}
		}
		fprintf(df, "\n");
	}

	atrv = HEAD_PFIFO(ptree->tinker);
	if (atrv != NULL) {
		fprintf(df, "tinker");
		for ( ; atrv != NULL; atrv = atrv->link) {
			NTP_INSIST(T_Double == atrv->type);
			fprintf(df, " %s %s", keyword(atrv->attr),
				normal_dtoa(atrv->value.d));
		}
		fprintf(df, "\n");
	}

	if (ptree->broadcastclient)
		fprintf(df, "broadcastclient\n");

	peern = HEAD_PFIFO(ptree->peers);
	for ( ; peern != NULL; peern = peern->link) {
		addr = peern->addr;
		fprintf(df, "%s", keyword(peern->host_mode));
		switch (addr->type) {
#ifdef DEBUG
		default:
			fprintf(df, "# dump error:\n"
				"# unknown peer family %d for:\n"
				"%s", addr->type,
				keyword(peern->host_mode));
			break;
#endif
		case AF_UNSPEC:
			break;

		case AF_INET:
			fprintf(df, " -4");
			break;

		case AF_INET6:
			fprintf(df, " -6");
			break;
		}
		fprintf(df, " %s", addr->address);
		
		if (peern->minpoll != 0)
			fprintf(df, " minpoll %u", peern->minpoll);

		if (peern->maxpoll != 0)
			fprintf(df, " maxpoll %u", peern->maxpoll);

		if (peern->ttl != 0) {
			if (strlen(addr->address) > 8
			    && !memcmp(addr->address, "127.127.", 8))
				fprintf(df, " mode %u", peern->ttl);
			else
				fprintf(df, " ttl %u", peern->ttl);
		}

		if (peern->peerversion != NTP_VERSION)
			fprintf(df, " version %u", peern->peerversion);

		if (peern->peerkey != 0)
			fprintf(df, " key %u", peern->peerkey);

		if (peern->group != NULL)
			fprintf(df, " ident \"%s\"", peern->group);

		atrv = HEAD_PFIFO(peern->peerflags);
		for ( ; atrv != NULL; atrv = atrv->link) {
			NTP_INSIST(T_Flag == atrv->attr);
			NTP_INSIST(T_Integer == atrv->type);
			fprintf(df, " %s", keyword(atrv->value.i));
		}

		fprintf(df, "\n");
		
		addr_opts = HEAD_PFIFO(ptree->fudge);
		for ( ; addr_opts != NULL; addr_opts = addr_opts->link) {
			peer_addr = peern->addr;
			fudge_addr = addr_opts->addr;

			s1 = peer_addr->address;
			s2 = fudge_addr->address;

			if (strcmp(s1, s2))
				continue;

			fprintf(df, "fudge %s", s1);

			for (atrv = HEAD_PFIFO(addr_opts->options);
			     atrv != NULL;
			     atrv = atrv->link) {

				switch (atrv->type) {
#ifdef DEBUG
				default:
					fprintf(df, "\n# dump error:\n"
						"# unknown fudge atrv->type %d\n"
						"fudge %s", atrv->type,
						s1);
					break;
#endif
				case T_Double:
					fprintf(df, " %s %s",
						keyword(atrv->attr),
						normal_dtoa(atrv->value.d));
					break;

				case T_Integer:
					fprintf(df, " %s %d",
						keyword(atrv->attr),
						atrv->value.i);
					break;

				case T_String:
					fprintf(df, " %s %s",
						keyword(atrv->attr),
						atrv->value.s);
					break;
				}
			}
			fprintf(df, "\n");
		}
	}

	addr = HEAD_PFIFO(ptree->manycastserver);
	if (addr != NULL) {
		fprintf(df, "manycastserver");
		for ( ; addr != NULL; addr = addr->link)
			fprintf(df, " %s", addr->address);
		fprintf(df, "\n");
	}

	addr = HEAD_PFIFO(ptree->multicastclient);
	if (addr != NULL) {
		fprintf(df, "multicastclient");
		for ( ; addr != NULL; addr = addr->link)
			fprintf(df, " %s", addr->address);
		fprintf(df, "\n");
	}

	
	for (unpeern = HEAD_PFIFO(ptree->unpeers);
	     unpeern != NULL;
	     unpeern = unpeern->link)
		fprintf(df, "unpeer %s\n", unpeern->addr->address);

	atrv = HEAD_PFIFO(ptree->mru_opts);
	if (atrv != NULL) {
		fprintf(df, "mru");
		for ( ;	atrv != NULL; atrv = atrv->link)
			fprintf(df, " %s %d", keyword(atrv->attr),
				atrv->value.i);
		fprintf(df, "\n");
	}

	atrv = HEAD_PFIFO(ptree->discard_opts);
	if (atrv != NULL) {
		fprintf(df, "discard");
		for ( ;	atrv != NULL; atrv = atrv->link)
			fprintf(df, " %s %d", keyword(atrv->attr),
				atrv->value.i);
		fprintf(df, "\n");
	}

	
	for (rest_node = HEAD_PFIFO(ptree->restrict_opts);
	     rest_node != NULL; 
	     rest_node = rest_node->link) {

		if (NULL == rest_node->addr) {
			s = "default";			
			flags = HEAD_PFIFO(rest_node->flags);
			for ( ; flags != NULL; flags = flags->link)
				if (T_Source == flags->i) {
					s = "source";
					break;
				}
		} else {
			s = rest_node->addr->address;
		}
		fprintf(df, "restrict %s", s);
		if (rest_node->mask != NULL)
			fprintf(df, " mask %s",
				rest_node->mask->address);
		flags = HEAD_PFIFO(rest_node->flags);
		for ( ; flags != NULL; flags = flags->link)
			if (T_Source != flags->i)
				fprintf(df, " %s", keyword(flags->i));
		fprintf(df, "\n");
	}

	rule_node = HEAD_PFIFO(ptree->nic_rules);
	for ( ; rule_node != NULL; rule_node = rule_node->link) {
		fprintf(df, "interface %s %s\n",
			keyword(rule_node->action),
			(rule_node->match_class)
			    ? keyword(rule_node->match_class)
			    : rule_node->if_name);
	}

	str_node = HEAD_PFIFO(ptree->phone);
	if (str_node != NULL) {
		fprintf(df, "phone");
		for ( ; str_node != NULL; str_node = str_node->link)
			fprintf(df, " \"%s\"", str_node->s);
		fprintf(df, "\n");
	}

	atrv = HEAD_PFIFO(ptree->qos);
	if (atrv != NULL) {
		fprintf(df, "qos");
		for (; atrv != NULL; atrv = atrv->link)
			fprintf(df, " %s", atrv->value.s);
		fprintf(df, "\n");
	}

	setv_node = HEAD_PFIFO(ptree->setvar);
	for ( ; setv_node != NULL; setv_node = setv_node->link) {
		s1 = quote_if_needed(setv_node->var);
		s2 = quote_if_needed(setv_node->val);
		fprintf(df, "setvar %s = %s", s1, s2);
		free(s1);
		free(s2);
		if (setv_node->isdefault)
			fprintf(df, " default");
		fprintf(df, "\n");
	}

	i_n = HEAD_PFIFO(ptree->ttl);
	if (i_n != NULL) {
		fprintf(df, "ttl");
		for( ; i_n != NULL; i_n = i_n->link)
			fprintf(df, " %d", i_n->i);
		fprintf(df, "\n");
	}
	
	addr_opts = HEAD_PFIFO(ptree->trap);
	for ( ; addr_opts != NULL; addr_opts = addr_opts->link) {
		addr = addr_opts->addr;
		fprintf(df, "trap %s", addr->address);
		atrv = HEAD_PFIFO(addr_opts->options);
		for ( ; atrv != NULL; atrv = atrv->link) {
			switch (atrv->attr) {
#ifdef DEBUG
			default:
				fprintf(df, "\n# dump error:\n"
					"# unknown trap token %d\n"
					"trap %s", atrv->attr,
					addr->address);
				break;
#endif
			case T_Port:
				fprintf(df, " port %d", atrv->value.i);
				break;

			case T_Interface:
				fprintf(df, " interface %s",
					atrv->value.s);
				break;
			}
		}
		fprintf(df, "\n");
	}

	return 0;
}