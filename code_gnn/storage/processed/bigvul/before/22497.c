unsigned long nr_uninterruptible(void)
{
	unsigned long i, sum = 0;

	for_each_possible_cpu(i)
		sum += cpu_rq(i)->nr_uninterruptible;

	 
	if (unlikely((long)sum < 0))
		sum = 0;

	return sum;
}
