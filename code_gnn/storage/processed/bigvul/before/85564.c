static void hns_ppe_exc_irq_en(struct hns_ppe_cb *ppe_cb, int en)
{
	u32 clr_vlue = 0xfffffffful;
	u32 msk_vlue = en ? 0xfffffffful : 0;  
	u32 vld_msk = 0;

	 
	dsaf_set_bit(vld_msk, 0, 1);
	dsaf_set_bit(vld_msk, 1, 1);
	dsaf_set_bit(vld_msk, 7, 1);

	 
	dsaf_write_dev(ppe_cb, PPE_RINT_REG, clr_vlue);

	 
	dsaf_write_dev(ppe_cb, PPE_INTEN_REG, msk_vlue & vld_msk);
}
