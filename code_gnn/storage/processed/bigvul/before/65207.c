int lockd_up(struct net *net)
{
	struct svc_serv *serv;
	int error;

	mutex_lock(&nlmsvc_mutex);

	serv = lockd_create_svc();
	if (IS_ERR(serv)) {
		error = PTR_ERR(serv);
		goto err_create;
	}

	error = lockd_up_net(serv, net);
	if (error < 0)
		goto err_net;

	error = lockd_start_svc(serv);
	if (error < 0)
		goto err_start;

	nlmsvc_users++;
	 
err_put:
	svc_destroy(serv);
err_create:
	mutex_unlock(&nlmsvc_mutex);
	return error;

err_start:
	lockd_down_net(serv, net);
err_net:
	lockd_unregister_notifiers();
	goto err_put;
}
