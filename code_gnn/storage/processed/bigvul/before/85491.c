static void ocfs2_dio_free_write_ctx(struct inode *inode,
				     struct ocfs2_dio_write_ctxt *dwc)
{
	ocfs2_free_unwritten_list(inode, &dwc->dw_zero_list);
	kfree(dwc);
}