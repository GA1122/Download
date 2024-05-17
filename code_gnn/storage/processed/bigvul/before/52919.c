static int get_a_ctxt(struct file *fp, const struct qib_user_info *uinfo,
		      unsigned alg)
{
	struct qib_devdata *udd = NULL;
	int ret = 0, devmax, npresent, nup, ndev, dusable = 0, i;
	u32 port = uinfo->spu_port, ctxt;

	devmax = qib_count_units(&npresent, &nup);
	if (!npresent) {
		ret = -ENXIO;
		goto done;
	}
	if (nup == 0) {
		ret = -ENETDOWN;
		goto done;
	}

	if (alg == QIB_PORT_ALG_ACROSS) {
		unsigned inuse = ~0U;

		 
		for (ndev = 0; ndev < devmax; ndev++) {
			struct qib_devdata *dd = qib_lookup(ndev);
			unsigned cused = 0, cfree = 0, pusable = 0;

			if (!dd)
				continue;
			if (port && port <= dd->num_pports &&
			    usable(dd->pport + port - 1))
				pusable = 1;
			else
				for (i = 0; i < dd->num_pports; i++)
					if (usable(dd->pport + i))
						pusable++;
			if (!pusable)
				continue;
			for (ctxt = dd->first_user_ctxt; ctxt < dd->cfgctxts;
			     ctxt++)
				if (dd->rcd[ctxt])
					cused++;
				else
					cfree++;
			if (cfree && cused < inuse) {
				udd = dd;
				inuse = cused;
			}
		}
		if (udd) {
			ret = choose_port_ctxt(fp, udd, port, uinfo);
			goto done;
		}
	} else {
		for (ndev = 0; ndev < devmax; ndev++) {
			struct qib_devdata *dd = qib_lookup(ndev);

			if (dd) {
				ret = choose_port_ctxt(fp, dd, port, uinfo);
				if (!ret)
					goto done;
				if (ret == -EBUSY)
					dusable++;
			}
		}
	}
	ret = dusable ? -EBUSY : -ENETDOWN;

done:
	return ret;
}