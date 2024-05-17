bool CheckLRUListOrder(const base::LinkedList<MemEntryImpl>& lru_list) {
  base::Time previous_last_use_time;
  for (base::LinkNode<MemEntryImpl>* node = lru_list.head();
       node != lru_list.end(); node = node->next()) {
    if (node->value()->GetLastUsed() < previous_last_use_time)
      return false;
    previous_last_use_time = node->value()->GetLastUsed();
  }
   return true;
 }
