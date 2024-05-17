static int security_compute_sid(u32 ssid,
				u32 tsid,
				u16 orig_tclass,
				u32 specified,
				const char *objname,
				u32 *out_sid,
				bool kern)
{
	struct class_datum *cladatum = NULL;
	struct context *scontext = NULL, *tcontext = NULL, newcontext;
	struct role_trans *roletr = NULL;
	struct avtab_key avkey;
	struct avtab_datum *avdatum;
	struct avtab_node *node;
	u16 tclass;
	int rc = 0;
	bool sock;

	if (!ss_initialized) {
		switch (orig_tclass) {
		case SECCLASS_PROCESS:  
			*out_sid = ssid;
			break;
		default:
			*out_sid = tsid;
			break;
		}
		goto out;
	}

	context_init(&newcontext);

	read_lock(&policy_rwlock);

	if (kern) {
		tclass = unmap_class(orig_tclass);
		sock = security_is_socket_class(orig_tclass);
	} else {
		tclass = orig_tclass;
		sock = security_is_socket_class(map_class(tclass));
	}

	scontext = sidtab_search(&sidtab, ssid);
	if (!scontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
		       __func__, ssid);
		rc = -EINVAL;
		goto out_unlock;
	}
	tcontext = sidtab_search(&sidtab, tsid);
	if (!tcontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
		       __func__, tsid);
		rc = -EINVAL;
		goto out_unlock;
	}

	if (tclass && tclass <= policydb.p_classes.nprim)
		cladatum = policydb.class_val_to_struct[tclass - 1];

	 
	switch (specified) {
	case AVTAB_TRANSITION:
	case AVTAB_CHANGE:
		if (cladatum && cladatum->default_user == DEFAULT_TARGET) {
			newcontext.user = tcontext->user;
		} else {
			 
			 
			newcontext.user = scontext->user;
		}
		break;
	case AVTAB_MEMBER:
		 
		newcontext.user = tcontext->user;
		break;
	}

	 
	if (cladatum && cladatum->default_role == DEFAULT_SOURCE) {
		newcontext.role = scontext->role;
	} else if (cladatum && cladatum->default_role == DEFAULT_TARGET) {
		newcontext.role = tcontext->role;
	} else {
		if ((tclass == policydb.process_class) || (sock == true))
			newcontext.role = scontext->role;
		else
			newcontext.role = OBJECT_R_VAL;
	}

	 
	if (cladatum && cladatum->default_type == DEFAULT_SOURCE) {
		newcontext.type = scontext->type;
	} else if (cladatum && cladatum->default_type == DEFAULT_TARGET) {
		newcontext.type = tcontext->type;
	} else {
		if ((tclass == policydb.process_class) || (sock == true)) {
			 
			newcontext.type = scontext->type;
		} else {
			 
			newcontext.type = tcontext->type;
		}
	}

	 
	avkey.source_type = scontext->type;
	avkey.target_type = tcontext->type;
	avkey.target_class = tclass;
	avkey.specified = specified;
	avdatum = avtab_search(&policydb.te_avtab, &avkey);

	 
	if (!avdatum) {
		node = avtab_search_node(&policydb.te_cond_avtab, &avkey);
		for (; node; node = avtab_search_node_next(node, specified)) {
			if (node->key.specified & AVTAB_ENABLED) {
				avdatum = &node->datum;
				break;
			}
		}
	}

	if (avdatum) {
		 
		newcontext.type = avdatum->data;
	}

	 
	if (objname)
		filename_compute_type(&policydb, &newcontext, scontext->type,
				      tcontext->type, tclass, objname);

	 
	if (specified & AVTAB_TRANSITION) {
		 
		for (roletr = policydb.role_tr; roletr; roletr = roletr->next) {
			if ((roletr->role == scontext->role) &&
			    (roletr->type == tcontext->type) &&
			    (roletr->tclass == tclass)) {
				 
				newcontext.role = roletr->new_role;
				break;
			}
		}
	}

	 
	rc = mls_compute_sid(scontext, tcontext, tclass, specified,
			     &newcontext, sock);
	if (rc)
		goto out_unlock;

	 
	if (!policydb_context_isvalid(&policydb, &newcontext)) {
		rc = compute_sid_handle_invalid_context(scontext,
							tcontext,
							tclass,
							&newcontext);
		if (rc)
			goto out_unlock;
	}
	 
	rc = sidtab_context_to_sid(&sidtab, &newcontext, out_sid);
out_unlock:
	read_unlock(&policy_rwlock);
	context_destroy(&newcontext);
out:
	return rc;
}
