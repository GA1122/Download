void hns_ppe_reset_common(struct dsaf_device *dsaf_dev, u8 ppe_common_index)
{
	u32 i;
	int ret;
	struct ppe_common_cb *ppe_common;

	ppe_common = dsaf_dev->ppe_common[ppe_common_index];
	ret = hns_ppe_common_init_hw(ppe_common);
	if (ret)
		return;

	for (i = 0; i < ppe_common->ppe_num; i++) {
		 
		if (dsaf_dev->mac_cb[i])
			hns_ppe_init_hw(&ppe_common->ppe_cb[i]);
	}

	ret = hns_rcb_common_init_hw(dsaf_dev->rcb_common[ppe_common_index]);
	if (ret)
		return;

	hns_rcb_common_init_commit_hw(dsaf_dev->rcb_common[ppe_common_index]);
}