static int corsair_usage_to_gkey(unsigned int usage)
{
	 
	if (usage >= 0xd0 && usage <= 0xdf)
		return usage - 0xd0 + 1;
	 
	if (usage >= 0xe8 && usage <= 0xe9)
		return usage - 0xe8 + 17;
	return 0;
}
