kwsincr (kwset_t kwset, char const *text, size_t len)
{
  struct trie *trie = kwset->trie;
  char const *trans = kwset->trans;

  text += len;

   
  while (len--)
    {
      unsigned char uc = *--text;
      unsigned char label = trans ? trans[uc] : uc;

       
      struct tree *link = trie->links;
      struct tree *links[DEPTH_SIZE];
      enum { L, R } dirs[DEPTH_SIZE];
      links[0] = (struct tree *) &trie->links;
      dirs[0] = L;
      int depth = 1;

      while (link && label != link->label)
        {
          links[depth] = link;
          if (label < link->label)
            dirs[depth++] = L, link = link->llink;
          else
            dirs[depth++] = R, link = link->rlink;
        }

       
      if (!link)
        {
          link = obstack_alloc (&kwset->obstack, sizeof *link);
          link->llink = NULL;
          link->rlink = NULL;
          link->trie = obstack_alloc (&kwset->obstack, sizeof *link->trie);
          link->trie->accepting = 0;
          link->trie->links = NULL;
          link->trie->parent = trie;
          link->trie->next = NULL;
          link->trie->fail = NULL;
          link->trie->depth = trie->depth + 1;
          link->trie->shift = 0;
          link->label = label;
          link->balance = 0;

           
          if (dirs[--depth] == L)
            links[depth]->llink = link;
          else
            links[depth]->rlink = link;

           
          while (depth && !links[depth]->balance)
            {
              if (dirs[depth] == L)
                --links[depth]->balance;
              else
                ++links[depth]->balance;
              --depth;
            }

           
          if (depth && ((dirs[depth] == L && --links[depth]->balance)
                        || (dirs[depth] == R && ++links[depth]->balance)))
            {
              struct tree *t, *r, *l, *rl, *lr;

              switch (links[depth]->balance)
                {
                case (char) -2:
                  switch (dirs[depth + 1])
                    {
                    case L:
                      r = links[depth], t = r->llink, rl = t->rlink;
                      t->rlink = r, r->llink = rl;
                      t->balance = r->balance = 0;
                      break;
                    case R:
                      r = links[depth], l = r->llink, t = l->rlink;
                      rl = t->rlink, lr = t->llink;
                      t->llink = l, l->rlink = lr, t->rlink = r, r->llink = rl;
                      l->balance = t->balance != 1 ? 0 : -1;
                      r->balance = t->balance != (char) -1 ? 0 : 1;
                      t->balance = 0;
                      break;
                    default:
                      abort ();
                    }
                  break;
                case 2:
                  switch (dirs[depth + 1])
                    {
                    case R:
                      l = links[depth], t = l->rlink, lr = t->llink;
                      t->llink = l, l->rlink = lr;
                      t->balance = l->balance = 0;
                      break;
                    case L:
                      l = links[depth], r = l->rlink, t = r->llink;
                      lr = t->llink, rl = t->rlink;
                      t->llink = l, l->rlink = lr, t->rlink = r, r->llink = rl;
                      l->balance = t->balance != 1 ? 0 : -1;
                      r->balance = t->balance != (char) -1 ? 0 : 1;
                      t->balance = 0;
                      break;
                    default:
                      abort ();
                    }
                  break;
                default:
                  abort ();
                }

              if (dirs[depth - 1] == L)
                links[depth - 1]->llink = t;
              else
                links[depth - 1]->rlink = t;
            }
        }

      trie = link->trie;
    }

   
  if (!trie->accepting)
    trie->accepting = 1 + 2 * kwset->words;
  ++kwset->words;

   
  if (trie->depth < kwset->mind)
    kwset->mind = trie->depth;
  if (trie->depth > kwset->maxd)
    kwset->maxd = trie->depth;
}