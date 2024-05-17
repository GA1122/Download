static inline void GetMaximumPixelList(PixelList *pixel_list,Quantum *pixel)
{
  register SkipList
    *p;

  size_t
    color,
    maximum;

  ssize_t
    count;

   
  p=(&pixel_list->skip_list);
  color=65536L;
  count=0;
  maximum=p->nodes[color].next[0];
  do
  {
    color=p->nodes[color].next[0];
    if (color > maximum)
      maximum=color;
    count+=p->nodes[color].count;
  } while (count < (ssize_t) pixel_list->length);
  *pixel=ScaleShortToQuantum((unsigned short) maximum);
}
