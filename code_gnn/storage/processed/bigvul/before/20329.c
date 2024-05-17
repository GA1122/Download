int is_invalid_pfn(pfn_t pfn)
{
	return pfn == hwpoison_pfn || pfn == fault_pfn;
}
