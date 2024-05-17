static int qib_assign_ctxt(struct file *fp, const struct qib_user_info *uinfo)
{
	int ret;
	int i_minor;
	unsigned swmajor, swminor, alg = QIB_PORT_ALG_ACROSS;

	 
	if (ctxt_fp(fp)) {
		ret = -EINVAL;
		goto done;
	}

	 
	swmajor = uinfo->spu_userversion >> 16;
	if (swmajor != QIB_USER_SWMAJOR) {
		ret = -ENODEV;
		goto done;
	}

	swminor = uinfo->spu_userversion & 0xffff;

	if (swminor >= 11 && uinfo->spu_port_alg < QIB_PORT_ALG_COUNT)
		alg = uinfo->spu_port_alg;

	mutex_lock(&qib_mutex);

	if (qib_compatible_subctxts(swmajor, swminor) &&
	    uinfo->spu_subctxt_cnt) {
		ret = find_shared_ctxt(fp, uinfo);
		if (ret > 0) {
			ret = do_qib_user_sdma_queue_create(fp);
			if (!ret)
				assign_ctxt_affinity(fp, (ctxt_fp(fp))->dd);
			goto done_ok;
		}
	}

	i_minor = iminor(file_inode(fp)) - QIB_USER_MINOR_BASE;
	if (i_minor)
		ret = find_free_ctxt(i_minor - 1, fp, uinfo);
	else {
		int unit;
		const unsigned int cpu = cpumask_first(&current->cpus_allowed);
		const unsigned int weight =
			cpumask_weight(&current->cpus_allowed);

		if (weight == 1 && !test_bit(cpu, qib_cpulist))
			if (!find_hca(cpu, &unit) && unit >= 0)
				if (!find_free_ctxt(unit, fp, uinfo)) {
					ret = 0;
					goto done_chk_sdma;
				}
		ret = get_a_ctxt(fp, uinfo, alg);
	}

done_chk_sdma:
	if (!ret)
		ret = do_qib_user_sdma_queue_create(fp);
done_ok:
	mutex_unlock(&qib_mutex);

done:
	return ret;
}