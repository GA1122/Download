treefails (struct tree const *tree, struct trie const *fail,
           struct trie *recourse)
{
  struct tree *link;

  if (!tree)
    return;

  treefails(tree->llink, fail, recourse);
  treefails(tree->rlink, fail, recourse);

   
  while (fail)
    {
      link = fail->links;
      while (link && tree->label != link->label)
        if (tree->label < link->label)
          link = link->llink;
        else
          link = link->rlink;
      if (link)
        {
          tree->trie->fail = link->trie;
          return;
        }
      fail = fail->fail;
    }

  tree->trie->fail = recourse;
}