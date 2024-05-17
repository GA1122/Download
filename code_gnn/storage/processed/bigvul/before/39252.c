static int convert_context(u32 key,
			   struct context *c,
			   void *p)
{
	struct convert_context_args *args;
	struct context oldc;
	struct ocontext *oc;
	struct mls_range *range;
	struct role_datum *role;
	struct type_datum *typdatum;
	struct user_datum *usrdatum;
	char *s;
	u32 len;
	int rc = 0;

	if (key <= SECINITSID_NUM)
		goto out;

	args = p;

	if (c->str) {
		struct context ctx;

		rc = -ENOMEM;
		s = kstrdup(c->str, GFP_KERNEL);
		if (!s)
			goto out;

		rc = string_to_context_struct(args->newp, NULL, s,
					      c->len, &ctx, SECSID_NULL);
		kfree(s);
		if (!rc) {
			printk(KERN_INFO "SELinux:  Context %s became valid (mapped).\n",
			       c->str);
			 
			kfree(c->str);
			memcpy(c, &ctx, sizeof(*c));
			goto out;
		} else if (rc == -EINVAL) {
			 
			rc = 0;
			goto out;
		} else {
			 
			printk(KERN_ERR "SELinux:   Unable to map context %s, rc = %d.\n",
			       c->str, -rc);
			goto out;
		}
	}

	rc = context_cpy(&oldc, c);
	if (rc)
		goto out;

	 
	rc = -EINVAL;
	usrdatum = hashtab_search(args->newp->p_users.table,
				  sym_name(args->oldp, SYM_USERS, c->user - 1));
	if (!usrdatum)
		goto bad;
	c->user = usrdatum->value;

	 
	rc = -EINVAL;
	role = hashtab_search(args->newp->p_roles.table,
			      sym_name(args->oldp, SYM_ROLES, c->role - 1));
	if (!role)
		goto bad;
	c->role = role->value;

	 
	rc = -EINVAL;
	typdatum = hashtab_search(args->newp->p_types.table,
				  sym_name(args->oldp, SYM_TYPES, c->type - 1));
	if (!typdatum)
		goto bad;
	c->type = typdatum->value;

	 
	if (args->oldp->mls_enabled && args->newp->mls_enabled) {
		rc = mls_convert_context(args->oldp, args->newp, c);
		if (rc)
			goto bad;
	} else if (args->oldp->mls_enabled && !args->newp->mls_enabled) {
		 
		mls_context_destroy(c);
	} else if (!args->oldp->mls_enabled && args->newp->mls_enabled) {
		 
		oc = args->newp->ocontexts[OCON_ISID];
		while (oc && oc->sid[0] != SECINITSID_UNLABELED)
			oc = oc->next;
		rc = -EINVAL;
		if (!oc) {
			printk(KERN_ERR "SELinux:  unable to look up"
				" the initial SIDs list\n");
			goto bad;
		}
		range = &oc->context[0].range;
		rc = mls_range_set(c, range);
		if (rc)
			goto bad;
	}

	 
	if (!policydb_context_isvalid(args->newp, c)) {
		rc = convert_context_handle_invalid_context(&oldc);
		if (rc)
			goto bad;
	}

	context_destroy(&oldc);

	rc = 0;
out:
	return rc;
bad:
	 
	rc = context_struct_to_string(&oldc, &s, &len);
	if (rc)
		return rc;
	context_destroy(&oldc);
	context_destroy(c);
	c->str = s;
	c->len = len;
	printk(KERN_INFO "SELinux:  Context %s became invalid (unmapped).\n",
	       c->str);
	rc = 0;
	goto out;
}
