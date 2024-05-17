cifs_rqst_page_to_kvec(struct smb_rqst *rqst, unsigned int idx,
			struct kvec *iov)
{
	 
	iov->iov_base = kmap(rqst->rq_pages[idx]);

	 
	if (idx == (rqst->rq_npages - 1))
		iov->iov_len = rqst->rq_tailsz;
	else
		iov->iov_len = rqst->rq_pagesz;
}
