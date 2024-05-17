 gs_heap_alloc_bytes(gs_memory_t * mem, uint size, client_name_t cname)
 {
     gs_malloc_memory_t *mmem = (gs_malloc_memory_t *) mem;
     byte *ptr = 0;
 
 #ifdef DEBUG
     const char *msg;
     static const char *const ok_msg = "OK";
 
 #  define set_msg(str) (msg = (str))
 #else
 #  define set_msg(str) DO_NOTHING
 #endif
 
          
     if (mmem->monitor)
         gx_monitor_enter(mmem->monitor);
     if (size > mmem->limit - sizeof(gs_malloc_block_t)) {
          
         set_msg("exceeded limit");
      } else {
          uint added = size + sizeof(gs_malloc_block_t);
  
        if (mmem->limit - added < mmem->used)
//         if (added <= size || mmem->limit - added < mmem->used)
              set_msg("exceeded limit");
          else if ((ptr = (byte *) Memento_label(malloc(added), cname)) == 0)
              set_msg("failed");
         else {
             gs_malloc_block_t *bp = (gs_malloc_block_t *) ptr;
 
              
             set_msg(ok_msg);
             if (mmem->allocated)
                 mmem->allocated->prev = bp;
             bp->next = mmem->allocated;
             bp->prev = 0;
             bp->size = size;
             bp->type = &st_bytes;
             bp->cname = cname;
             mmem->allocated = bp;
             ptr = (byte *) (bp + 1);
             mmem->used += size + sizeof(gs_malloc_block_t);
             if (mmem->used > mmem->max_used)
                 mmem->max_used = mmem->used;
         }
     }
     if (mmem->monitor)
         gx_monitor_leave(mmem->monitor);	 
      
     if (ptr)
         gs_alloc_fill(ptr, gs_alloc_fill_alloc, size);
 #ifdef DEBUG
     if (gs_debug_c('a') || msg != ok_msg)
         dmlprintf6(mem, "[a+]gs_malloc(%s)(%u) = 0x%lx: %s, used=%ld, max=%ld\n",
                    client_name_string(cname), size, (ulong) ptr, msg, mmem->used, mmem->max_used);
 #endif
     return ptr;
 #undef set_msg
 }