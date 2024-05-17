static inline int vmcs_field_width(unsigned long field)
{
	if (0x1 & field)	 
		return VMCS_FIELD_WIDTH_U32;
	return (field >> 13) & 0x3 ;
}