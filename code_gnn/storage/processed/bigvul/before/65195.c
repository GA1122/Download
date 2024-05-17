static int __init init_nlm(void)
{
	int err;

#ifdef CONFIG_SYSCTL
	err = -ENOMEM;
	nlm_sysctl_table = register_sysctl_table(nlm_sysctl_root);
	if (nlm_sysctl_table == NULL)
		goto err_sysctl;
#endif
	err = register_pernet_subsys(&lockd_net_ops);
	if (err)
		goto err_pernet;

	err = lockd_create_procfs();
	if (err)
		goto err_procfs;

	return 0;

err_procfs:
	unregister_pernet_subsys(&lockd_net_ops);
err_pernet:
#ifdef CONFIG_SYSCTL
	unregister_sysctl_table(nlm_sysctl_table);
err_sysctl:
#endif
	return err;
}
