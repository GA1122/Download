fbCombineConjointOutPart (CARD8 a, CARD8 b)
{
     
     

     

    if (b >= a)		     
	return 0x00;	     
    return ~FbIntDiv(b,a);    
}
