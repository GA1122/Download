void fill_post_wcc(struct svc_fh *fhp)
{
	__be32 err;

	if (fhp->fh_post_saved)
		printk("nfsd: inode locked twice during operation.\n");

	err = fh_getattr(fhp, &fhp->fh_post_attr);
	fhp->fh_post_change = d_inode(fhp->fh_dentry)->i_version;
	if (err) {
		fhp->fh_post_saved = false;
		 
		fhp->fh_post_attr.ctime = d_inode(fhp->fh_dentry)->i_ctime;
	} else
		fhp->fh_post_saved = true;
}