void hns_rcb_common_init_commit_hw(struct rcb_common_cb *rcb_common)
{
	wmb();	 
	dsaf_write_dev(rcb_common, RCB_COM_CFG_SYS_FSH_REG, 1);
	wmb();	 
}