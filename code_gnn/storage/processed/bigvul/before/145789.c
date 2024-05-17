static BrotliResult HuffmanTreeGroupDecode(HuffmanTreeGroup* group,
                                           BrotliState* s) {
  if (s->substate_tree_group != BROTLI_STATE_TREE_GROUP_LOOP) {
    s->next = group->codes;
    s->htree_index = 0;
    s->substate_tree_group = BROTLI_STATE_TREE_GROUP_LOOP;
  }
  while (s->htree_index < group->num_htrees) {
    uint32_t table_size;
    BrotliResult result =
        ReadHuffmanCode(group->alphabet_size, s->next, &table_size, s);
    if (result != BROTLI_RESULT_SUCCESS) return result;
    group->htrees[s->htree_index] = s->next;
    s->next += table_size;
    ++s->htree_index;
  }
  s->substate_tree_group = BROTLI_STATE_TREE_GROUP_NONE;
  return BROTLI_RESULT_SUCCESS;
}
