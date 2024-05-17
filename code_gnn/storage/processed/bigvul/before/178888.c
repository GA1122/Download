  static ssize_t __nfs4_get_acl_uncached(struct inode *inode, void *buf, size_t buflen)
  {
	struct page *pages[NFS4ACL_MAXPAGES];
// 	struct page *pages[NFS4ACL_MAXPAGES] = {NULL, };
  	struct nfs_getaclargs args = {
  		.fh = NFS_FH(inode),
  		.acl_pages = pages,
 		.acl_len = buflen,
 	};
 	struct nfs_getaclres res = {
 		.acl_len = buflen,
 	};
 	void *resp_buf;
 	struct rpc_message msg = {
 		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_GETACL],
  		.rpc_argp = &args,
  		.rpc_resp = &res,
  	};
	struct page *localpage = NULL;
	int ret;
// 	int ret = -ENOMEM, npages, i, acl_len = 0;
  
	if (buflen < PAGE_SIZE) {
// 	npages = (buflen + PAGE_SIZE - 1) >> PAGE_SHIFT;
// 	 
// 	if (npages == 0)
// 		npages = 1;
// 
// 	for (i = 0; i < npages; i++) {
// 		pages[i] = alloc_page(GFP_KERNEL);
// 		if (!pages[i])
// 			goto out_free;
// 	}
// 	if (npages > 1) {
 		 
		localpage = alloc_page(GFP_KERNEL);
		resp_buf = page_address(localpage);
		if (localpage == NULL)
			return -ENOMEM;
		args.acl_pages[0] = localpage;
		args.acl_pgbase = 0;
		args.acl_len = PAGE_SIZE;
	} else {
		resp_buf = buf;
		buf_to_pages(buf, buflen, args.acl_pages, &args.acl_pgbase);
// 		args.acl_scratch = alloc_page(GFP_KERNEL);
// 		if (!args.acl_scratch)
// 			goto out_free;
  	}
	ret = nfs4_call_sync(NFS_SERVER(inode)->client, NFS_SERVER(inode), &msg, &args.seq_args, &res.seq_res, 0);
// 	args.acl_len = npages * PAGE_SIZE;
// 	args.acl_pgbase = 0;
// 	 
// 	if (buf == NULL)
// 		res.acl_flags |= NFS4_ACL_LEN_REQUEST;
// 	resp_buf = page_address(pages[0]);
// 
// 	dprintk("%s  buf %p buflen %ld npages %d args.acl_len %ld\n",
// 		__func__, buf, buflen, npages, args.acl_len);
// 	ret = nfs4_call_sync(NFS_SERVER(inode)->client, NFS_SERVER(inode),
// 			     &msg, &args.seq_args, &res.seq_res, 0);
  	if (ret)
  		goto out_free;
	if (res.acl_len > args.acl_len)
		nfs4_write_cached_acl(inode, NULL, res.acl_len);
// 
// 	acl_len = res.acl_len - res.acl_data_offset;
// 	if (acl_len > args.acl_len)
// 		nfs4_write_cached_acl(inode, NULL, acl_len);
  	else
		nfs4_write_cached_acl(inode, resp_buf, res.acl_len);
// 		nfs4_write_cached_acl(inode, resp_buf + res.acl_data_offset,
// 				      acl_len);
  	if (buf) {
  		ret = -ERANGE;
		if (res.acl_len > buflen)
// 		if (acl_len > buflen)
  			goto out_free;
		if (localpage)
			memcpy(buf, resp_buf, res.acl_len);
// 		_copy_from_pages(buf, pages, res.acl_data_offset,
// 				res.acl_len);
  	}
	ret = res.acl_len;
// 	ret = acl_len;
  out_free:
	if (localpage)
		__free_page(localpage);
// 	for (i = 0; i < npages; i++)
// 		if (pages[i])
// 			__free_page(pages[i]);
// 	if (args.acl_scratch)
// 		__free_page(args.acl_scratch);
  	return ret;
  }