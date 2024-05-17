static void AddNodePixelList(PixelList *pixel_list,const size_t color)
{
  register SkipList
    *p;

  register ssize_t
    level;

  size_t
    search,
    update[9];

   
  p=(&pixel_list->skip_list);
  p->nodes[color].signature=pixel_list->signature;
  p->nodes[color].count=1;
   
  search=65536UL;
  for (level=p->level; level >= 0; level--)
  {
    while (p->nodes[search].next[level] < color)
      search=p->nodes[search].next[level];
    update[level]=search;
  }
   
  for (level=0; ; level++)
  {
    pixel_list->seed=(pixel_list->seed*42893621L)+1L;
    if ((pixel_list->seed & 0x300) != 0x300)
      break;
  }
  if (level > 8)
    level=8;
  if (level > (p->level+2))
    level=p->level+2;
   
  while (level > p->level)
  {
    p->level++;
    update[p->level]=65536UL;
  }
   
  do
  {
    p->nodes[color].next[level]=p->nodes[update[level]].next[level];
    p->nodes[update[level]].next[level]=color;
  } while (level-- > 0);
}
