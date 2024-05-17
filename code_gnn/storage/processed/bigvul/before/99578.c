void SBChunkList::clear() {
  for (std::vector<SBChunk>::iterator citer = chunks_.begin();
       citer != chunks_.end(); ++citer) {
    for (std::deque<SBChunkHost>::iterator hiter = citer->hosts.begin();
         hiter != citer->hosts.end(); ++hiter) {
      if (hiter->entry) {
        hiter->entry->Destroy();
        hiter->entry = NULL;
      }
    }
  }
  chunks_.clear();
}
