static int fast_bap_read(struct airo_info *ai, __le16 *pu16Dst,
			 int bytelen, int whichbap)
{
	bytelen = (bytelen + 1) & (~1);  
	if ( !do8bitIO )
		insw( ai->dev->base_addr+DATA0+whichbap, pu16Dst, bytelen>>1 );
	else
		insb( ai->dev->base_addr+DATA0+whichbap, pu16Dst, bytelen );
	return SUCCESS;
}