server_getlocation(struct client *clt, const char *path)
{
	struct server		*srv = clt->clt_srv;
	struct server_config	*srv_conf = clt->clt_srv_conf, *location;
	const char		*errstr = NULL;
	int			 ret;

	 
	TAILQ_FOREACH(location, &srv->srv_hosts, entry) {
#ifdef DEBUG
		if (location->flags & SRVFLAG_LOCATION) {
			DPRINTF("%s: location \"%s\" path \"%s\"",
			    __func__, location->location, path);
		}
#endif
		if ((location->flags & SRVFLAG_LOCATION) &&
		    location->parent_id == srv_conf->parent_id) {
			errstr = NULL;
			if (location->flags & SRVFLAG_LOCATION_MATCH) {
				ret = str_match(path, location->location,
				    &clt->clt_srv_match, &errstr);
			} else {
				ret = fnmatch(location->location,
				    path, FNM_CASEFOLD);
			}
			if (ret == 0 && errstr == NULL) {
				 
				clt->clt_srv_conf = srv_conf = location;
				break;
			}
		}
	}

	return (srv_conf);
}