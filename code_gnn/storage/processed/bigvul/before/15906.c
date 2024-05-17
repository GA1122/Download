static void AdjustYearIfNeeded(short &year)
{
	if (year<100)
	{
		if (year >40)
		{
			year=1900+year;
		}
		else
		{
			year=2000+year;
		}
	}
}
