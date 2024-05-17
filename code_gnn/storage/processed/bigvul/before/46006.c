devzvol_lookup(struct vnode *dvp, char *nm, struct vnode **vpp,
    struct pathname *pnp, int flags, struct vnode *rdir, struct cred *cred,
    caller_context_t *ct, int *direntflags, pathname_t *realpnp)
{
	enum vtype expected_type = VDIR;
	struct sdev_node *parent = VTOSDEV(dvp);
	char *dsname;
	dmu_objset_type_t do_type;
	int error;

	sdcmn_err13(("devzvol_lookup '%s' '%s'", parent->sdev_path, nm));
	*vpp = NULL;
	 
	if ((error = VOP_ACCESS(dvp, VEXEC, 0, cred, ct)) != 0)
		return (error);

	rw_enter(&parent->sdev_contents, RW_READER);
	if (SDEV_IS_GLOBAL(parent)) {
		 
		if (getzoneid() != GLOBAL_ZONEID) {
			rw_exit(&parent->sdev_contents);
			return (EPERM);
		}
	} else {
		int res;

		rw_exit(&parent->sdev_contents);

		 
		if (getzoneid() == GLOBAL_ZONEID)
			return (EPERM);

		res = prof_lookup(dvp, nm, vpp, cred);

		 
		if (res == ENOENT) {
			 
			if (devzvol_mk_ngz_node(parent, nm) != 0)
				return (ENOENT);
			res = prof_lookup(dvp, nm, vpp, cred);
		}

		return (res);
	}

	dsname = devzvol_make_dsname(parent->sdev_path, nm);
	rw_exit(&parent->sdev_contents);
	sdcmn_err13(("rvp dsname %s", dsname ? dsname : "(null)"));
	if (dsname) {
		error = devzvol_objset_check(dsname, &do_type);
		if (error != 0) {
			error = ENOENT;
			goto out;
		}
		if (do_type == DMU_OST_ZVOL)
			expected_type = VLNK;
	}
	 
	if (expected_type == VDIR) {
		error = devname_lookup_func(parent, nm, vpp, cred,
		    devzvol_create_dir, SDEV_VATTR);
	} else {
		error = devname_lookup_func(parent, nm, vpp, cred,
		    devzvol_create_link, SDEV_VLINK);
	}
	sdcmn_err13(("devzvol_lookup %d %d", expected_type, error));
	ASSERT(error || ((*vpp)->v_type == expected_type));
out:
	if (dsname)
		kmem_free(dsname, strlen(dsname) + 1);
	sdcmn_err13(("devzvol_lookup %d", error));
	return (error);
}
