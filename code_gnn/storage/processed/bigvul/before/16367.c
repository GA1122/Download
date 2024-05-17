pseudo_get_job_attr( const char *name, MyString &expr )
{
	RemoteResource *remote;
	if (parallelMasterResource == NULL) {
		remote = thisRemoteResource;
	} else {
		remote = parallelMasterResource;
	}
	ClassAd *ad = remote->getJobAd();

	ExprTree *e = ad->LookupExpr(name);
	if(e) {
		expr = ExprTreeToString(e);
		dprintf(D_SYSCALLS,"pseudo_get_job_attr(%s) = %s\n",name,expr.Value());
		return 0;
	} else {
		dprintf(D_SYSCALLS,"pseudo_get_job_attr(%s) is UNDEFINED\n",name);
		expr = "UNDEFINED";
		return 0;
	}
}
