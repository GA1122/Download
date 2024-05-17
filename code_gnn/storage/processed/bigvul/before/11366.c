fbCombineDisjointInPart (CARD8 a, CARD8 b)
{
     
     
     

    b = ~b;		     
    if (b >= a)		     
	return 0;	     
    return ~FbIntDiv(b,a);   
}
