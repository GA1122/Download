 void check_request_for_cacheability(struct stream *s, struct channel *chn)
 {
 	struct http_txn *txn = s->txn;
 	char *p1, *p2;
 	char *cur_ptr, *cur_end, *cur_next;
 	int pragma_found;
 	int cc_found;
 	int cur_idx;
 
 	if ((txn->flags & (TX_CACHEABLE|TX_CACHE_IGNORE)) == TX_CACHE_IGNORE)
 		return;  
 
 	cur_idx = 0;
 	pragma_found = cc_found = 0;
 	cur_next = chn->buf->p + hdr_idx_first_pos(&txn->hdr_idx);
 
 	while ((cur_idx = txn->hdr_idx.v[cur_idx].next)) {
 		struct hdr_idx_elem *cur_hdr;
 		int val;
 
 		cur_hdr  = &txn->hdr_idx.v[cur_idx];
 		cur_ptr  = cur_next;
 		cur_end  = cur_ptr + cur_hdr->len;
 		cur_next = cur_end + cur_hdr->cr + 1;
 
 		 
 
 		val = http_header_match2(cur_ptr, cur_end, "Pragma", 6);
 		if (val) {
 			if ((cur_end - (cur_ptr + val) >= 8) &&
 			    strncasecmp(cur_ptr + val, "no-cache", 8) == 0) {
 				pragma_found = 1;
 				continue;
                         }
                 }
  
//                 
//                val = http_header_match2(cur_ptr, cur_end, "Authorization", 13);
//                if (val) {
//                        txn->flags &= ~TX_CACHEABLE & ~TX_CACHE_COOK;
//                        txn->flags |= TX_CACHE_IGNORE;
//                        continue;
//                }
// 
                 val = http_header_match2(cur_ptr, cur_end, "Cache-control", 13);
                 if (!val)
                         continue;
 		p2 = p1;
 		while (p2 < cur_end && *p2 != '=' && *p2 != ',' && !isspace((unsigned char)*p2))
 			p2++;
 
 		 
 		if (((p2 - p1 == 7) && strncasecmp(p1, "max-age",   7) == 0) ||
 		    ((p2 - p1 == 8) && strncasecmp(p1, "no-cache",  8) == 0) ||
 		    ((p2 - p1 == 9) && strncasecmp(p1, "max-stale", 9) == 0) ||
 		    ((p2 - p1 == 9) && strncasecmp(p1, "min-fresh", 9) == 0)) {
 			txn->flags |= TX_CACHE_IGNORE;
 			continue;
 		}
 
 		if ((p2 - p1 == 8) && strncasecmp(p1, "no-store", 8) == 0) {
 			txn->flags &= ~TX_CACHEABLE & ~TX_CACHE_COOK;
 			continue;
 		}
 	}
 
 	 
 	if (!cc_found && pragma_found)
 		txn->flags |= TX_CACHE_IGNORE;
 }