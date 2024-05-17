int userns_exec_1(struct lxc_conf *conf, int (*fn)(void *), void *data)
{
	int ret, pid;
	struct userns_fn_data d;
	char c = '1';
	int p[2];
	struct lxc_list *idmap;

	ret = pipe(p);
	if (ret < 0) {
		SYSERROR("opening pipe");
		return -1;
	}
	d.fn = fn;
	d.arg = data;
	d.p[0] = p[0];
	d.p[1] = p[1];
	pid = lxc_clone(run_userns_fn, &d, CLONE_NEWUSER);
	if (pid < 0)
		goto err;
	close(p[0]);
	p[0] = -1;

	if ((idmap = idmap_add_id(conf, geteuid(), getegid())) == NULL) {
		ERROR("Error adding self to container uid/gid map");
		goto err;
	}

	ret = lxc_map_ids(idmap, pid);
	lxc_free_idmap(idmap);
	free(idmap);
	if (ret) {
		ERROR("Error setting up child mappings");
		goto err;
	}

	if (write(p[1], &c, 1) != 1) {
		SYSERROR("writing to pipe to child");
		goto err;
	}

	ret = wait_for_pid(pid);

	close(p[1]);
	return ret;

err:
	if (p[0] != -1)
		close(p[0]);
	close(p[1]);
	return -1;
}