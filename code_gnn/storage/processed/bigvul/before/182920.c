  static int mincore_unmapped_range(unsigned long addr, unsigned long end,
  				   struct mm_walk *walk)
  {
	walk->private += __mincore_unmapped_range(addr, end,
						  walk->vma, walk->private);
// 	unsigned char *vec = walk->private;
// 	unsigned long nr = (end - addr) >> PAGE_SHIFT;
// 
// 	memset(vec, 0, nr);
// 	walk->private += nr;
  	return 0;
  }