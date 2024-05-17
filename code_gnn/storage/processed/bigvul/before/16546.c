CronTab::sort( ExtArray<int> &list )
{
	int ctr, ctr2, value;
	for ( ctr = 1; ctr <= list.getlast(); ctr++ ) {
		value = list[ctr];
    	ctr2 = ctr;
		while ( ( ctr2 > 0 ) && ( list[ctr2 - 1] > value ) ) {
			list[ctr2] = list[ctr2 - 1];
			ctr2--;
		}  
		list[ctr2] = value;
	}  
	return;
}
