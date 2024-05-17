 int rds_cmsg_atomic(struct rds_sock *rs, struct rds_message *rm,
 		    struct cmsghdr *cmsg)
 {
 	struct page *page = NULL;
 	struct rds_atomic_args *args;
 	int ret = 0;
 
 	if (cmsg->cmsg_len < CMSG_LEN(sizeof(struct rds_atomic_args))
 	 || rm->atomic.op_active)
 		return -EINVAL;
 
 	args = CMSG_DATA(cmsg);
 
 	 
 	switch (cmsg->cmsg_type) {
 	case RDS_CMSG_ATOMIC_FADD:
 		rm->atomic.op_type = RDS_ATOMIC_TYPE_FADD;
 		rm->atomic.op_m_fadd.add = args->fadd.add;
 		rm->atomic.op_m_fadd.nocarry_mask = 0;
 		break;
 	case RDS_CMSG_MASKED_ATOMIC_FADD:
 		rm->atomic.op_type = RDS_ATOMIC_TYPE_FADD;
 		rm->atomic.op_m_fadd.add = args->m_fadd.add;
 		rm->atomic.op_m_fadd.nocarry_mask = args->m_fadd.nocarry_mask;
 		break;
 	case RDS_CMSG_ATOMIC_CSWP:
 		rm->atomic.op_type = RDS_ATOMIC_TYPE_CSWP;
 		rm->atomic.op_m_cswp.compare = args->cswp.compare;
 		rm->atomic.op_m_cswp.swap = args->cswp.swap;
 		rm->atomic.op_m_cswp.compare_mask = ~0;
 		rm->atomic.op_m_cswp.swap_mask = ~0;
 		break;
 	case RDS_CMSG_MASKED_ATOMIC_CSWP:
 		rm->atomic.op_type = RDS_ATOMIC_TYPE_CSWP;
 		rm->atomic.op_m_cswp.compare = args->m_cswp.compare;
 		rm->atomic.op_m_cswp.swap = args->m_cswp.swap;
 		rm->atomic.op_m_cswp.compare_mask = args->m_cswp.compare_mask;
 		rm->atomic.op_m_cswp.swap_mask = args->m_cswp.swap_mask;
 		break;
 	default:
 		BUG();  
 	}
 
 	rm->atomic.op_notify = !!(args->flags & RDS_RDMA_NOTIFY_ME);
 	rm->atomic.op_silent = !!(args->flags & RDS_RDMA_SILENT);
 	rm->atomic.op_active = 1;
 	rm->atomic.op_recverr = rs->rs_recverr;
 	rm->atomic.op_sg = rds_message_alloc_sgs(rm, 1);
 	if (!rm->atomic.op_sg) {
 		ret = -ENOMEM;
 		goto err;
 	}
 
 	 
 	if (args->local_addr & 0x7) {
 		ret = -EFAULT;
 		goto err;
 	}
 
 	ret = rds_pin_pages(args->local_addr, 1, &page, 1);
 	if (ret != 1)
 		goto err;
 	ret = 0;
 
 	sg_set_page(rm->atomic.op_sg, page, 8, offset_in_page(args->local_addr));
 
 	if (rm->atomic.op_notify || rm->atomic.op_recverr) {
 		 
 		rm->atomic.op_notifier = kmalloc(sizeof(*rm->atomic.op_notifier), GFP_KERNEL);
 		if (!rm->atomic.op_notifier) {
 			ret = -ENOMEM;
 			goto err;
 		}
 
 		rm->atomic.op_notifier->n_user_token = args->user_token;
 		rm->atomic.op_notifier->n_status = RDS_RDMA_SUCCESS;
 	}
 
 	rm->atomic.op_rkey = rds_rdma_cookie_key(args->cookie);
 	rm->atomic.op_remote_addr = args->remote_addr + rds_rdma_cookie_offset(args->cookie);
 
 	return ret;
  err:
  	if (page)
  		put_page(page);
// 	rm->atomic.op_active = 0;
  	kfree(rm->atomic.op_notifier);
  
  	return ret;
 }