void dns_unlink_resolution(struct dns_requester *requester)
{
	struct dns_resolution *res;
	struct dns_requester  *req;

	 
	if (!requester || !requester->resolution)
		return;
	res = requester->resolution;

	 
	LIST_DEL(&requester->list);
	requester->resolution = NULL;

	 
	if (!LIST_ISEMPTY(&res->requesters))
		req = LIST_NEXT(&res->requesters, struct dns_requester *, list);
	else {
		dns_free_resolution(res);
		return;
	}

	 
	switch (obj_type(req->owner)) {
		case OBJ_TYPE_SERVER:
			res->hostname_dn     = __objt_server(req->owner)->hostname_dn;
			res->hostname_dn_len = __objt_server(req->owner)->hostname_dn_len;
			break;
		case OBJ_TYPE_SRVRQ:
			res->hostname_dn     = __objt_dns_srvrq(req->owner)->hostname_dn;
			res->hostname_dn_len = __objt_dns_srvrq(req->owner)->hostname_dn_len;
			break;
		default:
			res->hostname_dn     = NULL;
			res->hostname_dn_len = 0;
			break;
	}
}
