static void qib_clean_part_key(struct qib_ctxtdata *rcd,
			       struct qib_devdata *dd)
{
	int i, j, pchanged = 0;
	u64 oldpkey;
	struct qib_pportdata *ppd = rcd->ppd;

	 
	oldpkey = (u64) ppd->pkeys[0] |
		((u64) ppd->pkeys[1] << 16) |
		((u64) ppd->pkeys[2] << 32) |
		((u64) ppd->pkeys[3] << 48);

	for (i = 0; i < ARRAY_SIZE(rcd->pkeys); i++) {
		if (!rcd->pkeys[i])
			continue;
		for (j = 0; j < ARRAY_SIZE(ppd->pkeys); j++) {
			 
			if ((ppd->pkeys[j] & 0x7fff) !=
			    (rcd->pkeys[i] & 0x7fff))
				continue;
			if (atomic_dec_and_test(&ppd->pkeyrefs[j])) {
				ppd->pkeys[j] = 0;
				pchanged++;
			}
			break;
		}
		rcd->pkeys[i] = 0;
	}
	if (pchanged)
		(void) ppd->dd->f_set_ib_cfg(ppd, QIB_IB_CFG_PKEYS, 0);
}