 nfsd_cross_mnt(struct svc_rqst *rqstp, struct dentry **dpp, 
 		        struct svc_export **expp)
 {
 	struct svc_export *exp = *expp, *exp2 = NULL;
 	struct dentry *dentry = *dpp;
 	struct path path = {.mnt = mntget(exp->ex_path.mnt),
 			    .dentry = dget(dentry)};
 	int err = 0;
 
  	err = follow_down(&path);
  	if (err < 0)
  		goto out;
// 	if (path.mnt == exp->ex_path.mnt && path.dentry == dentry &&
// 	    nfsd_mountpoint(dentry, exp) == 2) {
// 		 
// 		path_put(&path);
// 		goto out;
// 	}
  
  	exp2 = rqst_exp_get_by_name(rqstp, &path);
  	if (IS_ERR(exp2)) {
 		err = PTR_ERR(exp2);
 		 
 		if (err == -ENOENT && !(exp->ex_flags & NFSEXP_V4ROOT))
 			err = 0;
 		path_put(&path);
 		goto out;
 	}
 	if (nfsd_v4client(rqstp) ||
 		(exp->ex_flags & NFSEXP_CROSSMOUNT) || EX_NOHIDE(exp2)) {
 		 
 		 
 		*dpp = path.dentry;
 		path.dentry = dentry;
 		*expp = exp2;
 		exp2 = exp;
 	}
 	path_put(&path);
 	exp_put(exp2);
 out:
 	return err;
 }