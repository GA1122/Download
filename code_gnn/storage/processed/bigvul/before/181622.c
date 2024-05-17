 entry_guard_obeys_restriction(const entry_guard_t *guard,
                               const entry_guard_restriction_t *rst)
 {
   tor_assert(guard);
    if (! rst)
      return 1;  
  
//    
//    
//   const node_t *node = node_get_by_id((const char*)rst->exclude_id);
//   if (node && guard_in_node_family(guard, node))
//     return 0;
// 
    return tor_memneq(guard->identity, rst->exclude_id, DIGEST_LEN);
  }