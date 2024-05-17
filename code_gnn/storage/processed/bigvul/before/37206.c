static inline int vmcs_field_type(unsigned long field)
{
	if (0x1 & field)	 
		return VMCS_FIELD_TYPE_U32;
	return (field >> 13) & 0x3 ;
}
