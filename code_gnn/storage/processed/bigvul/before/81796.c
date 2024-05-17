int streamIteratorGetID(streamIterator *si, streamID *id, int64_t *numfields) {
    while(1) {  
         
        if (si->lp == NULL || si->lp_ele == NULL) {
            if (!si->rev && !raxNext(&si->ri)) return 0;
            else if (si->rev && !raxPrev(&si->ri)) return 0;
            serverAssert(si->ri.key_len == sizeof(streamID));
             
            streamDecodeID(si->ri.key,&si->master_id);
             
            si->lp = si->ri.data;
            si->lp_ele = lpFirst(si->lp);            
            si->lp_ele = lpNext(si->lp,si->lp_ele);  
            si->lp_ele = lpNext(si->lp,si->lp_ele);  
            si->master_fields_count = lpGetInteger(si->lp_ele);
            si->lp_ele = lpNext(si->lp,si->lp_ele);  
            si->master_fields_start = si->lp_ele;
             
            for (uint64_t i = 0; i < si->master_fields_count; i++)
                si->lp_ele = lpNext(si->lp,si->lp_ele);
             
            if (si->rev) si->lp_ele = lpLast(si->lp);
        } else if (si->rev) {
             
            int lp_count = lpGetInteger(si->lp_ele);
            while(lp_count--) si->lp_ele = lpPrev(si->lp,si->lp_ele);
             
            si->lp_ele = lpPrev(si->lp,si->lp_ele);
        }

         
        while(1) {
            if (!si->rev) {
                 
                si->lp_ele = lpNext(si->lp,si->lp_ele);
                if (si->lp_ele == NULL) break;
            } else {
                 
                int lp_count = lpGetInteger(si->lp_ele);
                if (lp_count == 0) {  
                    si->lp = NULL;
                    si->lp_ele = NULL;
                    break;
                }
                while(lp_count--) si->lp_ele = lpPrev(si->lp,si->lp_ele);
            }

             
            si->lp_flags = si->lp_ele;
            int flags = lpGetInteger(si->lp_ele);
            si->lp_ele = lpNext(si->lp,si->lp_ele);  

             
            *id = si->master_id;
            id->ms += lpGetInteger(si->lp_ele);
            si->lp_ele = lpNext(si->lp,si->lp_ele);
            id->seq += lpGetInteger(si->lp_ele);
            si->lp_ele = lpNext(si->lp,si->lp_ele);
            unsigned char buf[sizeof(streamID)];
            streamEncodeID(buf,id);

             
            if (flags & STREAM_ITEM_FLAG_SAMEFIELDS) {
                *numfields = si->master_fields_count;
            } else {
                *numfields = lpGetInteger(si->lp_ele);
                si->lp_ele = lpNext(si->lp,si->lp_ele);
            }

             
            if (!si->rev) {
                if (memcmp(buf,si->start_key,sizeof(streamID)) >= 0 &&
                    !(flags & STREAM_ITEM_FLAG_DELETED))
                {
                    if (memcmp(buf,si->end_key,sizeof(streamID)) > 0)
                        return 0;  
                    si->entry_flags = flags;
                    if (flags & STREAM_ITEM_FLAG_SAMEFIELDS)
                        si->master_fields_ptr = si->master_fields_start;
                    return 1;  
                }
            } else {
                if (memcmp(buf,si->end_key,sizeof(streamID)) <= 0 &&
                    !(flags & STREAM_ITEM_FLAG_DELETED))
                {
                    if (memcmp(buf,si->start_key,sizeof(streamID)) < 0)
                        return 0;  
                    si->entry_flags = flags;
                    if (flags & STREAM_ITEM_FLAG_SAMEFIELDS)
                        si->master_fields_ptr = si->master_fields_start;
                    return 1;  
                }
            }

             
            if (!si->rev) {
                int to_discard = (flags & STREAM_ITEM_FLAG_SAMEFIELDS) ?
                                    *numfields : *numfields*2;
                for (int64_t i = 0; i < to_discard; i++)
                    si->lp_ele = lpNext(si->lp,si->lp_ele);
            } else {
                int prev_times = 4;  
                while(prev_times--) si->lp_ele = lpPrev(si->lp,si->lp_ele);
            }
        }

         
    }
}
