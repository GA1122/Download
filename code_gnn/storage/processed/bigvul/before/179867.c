 static unsigned int stack_maxrandom_size(void)
// static unsigned long stack_maxrandom_size(void)
  {
	unsigned int max = 0;
// 	unsigned long max = 0;
  	if ((current->flags & PF_RANDOMIZE) &&
  		!(current->personality & ADDR_NO_RANDOMIZE)) {
		max = ((-1U) & STACK_RND_MASK) << PAGE_SHIFT;
// 		max = ((-1UL) & STACK_RND_MASK) << PAGE_SHIFT;
  	}
  
  	return max;
 }