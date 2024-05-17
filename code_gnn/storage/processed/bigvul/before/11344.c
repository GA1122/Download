fbCombineConjointInPart (CARD8 a, CARD8 b)
{
     

    if (b >= a)		     
	return 0xff;	     
    return FbIntDiv(b,a);    
}
