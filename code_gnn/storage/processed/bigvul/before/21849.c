static int test_cc(unsigned int condition, unsigned int flags)
{
	int rc = 0;

	switch ((condition & 15) >> 1) {
	case 0:  
		rc |= (flags & EFLG_OF);
		break;
	case 1:  
		rc |= (flags & EFLG_CF);
		break;
	case 2:  
		rc |= (flags & EFLG_ZF);
		break;
	case 3:  
		rc |= (flags & (EFLG_CF|EFLG_ZF));
		break;
	case 4:  
		rc |= (flags & EFLG_SF);
		break;
	case 5:  
		rc |= (flags & EFLG_PF);
		break;
	case 7:  
		rc |= (flags & EFLG_ZF);
		 
	case 6:  
		rc |= (!(flags & EFLG_SF) != !(flags & EFLG_OF));
		break;
	}

	 
	return (!!rc ^ (condition & 1));
}
