static void __init reserve_brk(void)
{
	if (_brk_end > _brk_start)
		memblock_reserve(__pa_symbol(_brk_start),
				 _brk_end - _brk_start);

	 
	_brk_start = 0;
}