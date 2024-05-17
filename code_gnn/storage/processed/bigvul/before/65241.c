static int nfs_callback_authenticate(struct svc_rqst *rqstp)
{
	switch (rqstp->rq_authop->flavour) {
	case RPC_AUTH_NULL:
		if (rqstp->rq_proc != CB_NULL)
			return SVC_DENIED;
		break;
	case RPC_AUTH_GSS:
		 
		 if (svc_is_backchannel(rqstp))
			return SVC_DENIED;
	}
	return SVC_OK;
}