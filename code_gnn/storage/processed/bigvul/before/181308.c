 nlmsvc_grant_reply(struct nlm_cookie *cookie, __be32 status)
 {
 	struct nlm_block	*block;
 
 	dprintk("grant_reply: looking for cookie %x, s=%d \n",
 		*(unsigned int *)(cookie->data), status);
  	if (!(block = nlmsvc_find_block(cookie)))
  		return;
  
	if (block) {
		if (status == nlm_lck_denied_grace_period) {
			 
			nlmsvc_insert_block(block, 10 * HZ);
		} else {
			 
			nlmsvc_unlink_block(block);
		}
// 	if (status == nlm_lck_denied_grace_period) {
// 		 
// 		nlmsvc_insert_block(block, 10 * HZ);
// 	} else {
// 		 
// 		nlmsvc_unlink_block(block);
  	}
  	nlmsvc_release_block(block);
  }