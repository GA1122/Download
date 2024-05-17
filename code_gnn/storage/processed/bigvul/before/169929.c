xsltFormatNumberPreSuffix(xsltDecimalFormatPtr self, xmlChar **format, xsltFormatNumberInfoPtr info)
{
    int	count=0;	 
    int len;

    while (1) {
	 
	if (**format == 0)
	    return count;
	 
	if (**format == SYMBOL_QUOTE) {
	    if (*++(*format) == 0)
		return -1;
	}
	else if (IS_SPECIAL(self, *format))
	    return count;
	 
	else {
	     
	    if (xsltUTF8Charcmp(*format, self->percent) == 0) {
		if (info->is_multiplier_set)
		    return -1;
		info->multiplier = 100;
		info->is_multiplier_set = TRUE;
	    } else if (xsltUTF8Charcmp(*format, self->permille) == 0) {
		if (info->is_multiplier_set)
		    return -1;
		info->multiplier = 1000;
		info->is_multiplier_set = TRUE;
	    }
	}

	if ((len=xsltUTF8Size(*format)) < 1)
	    return -1;
	count += len;
	*format += len;
    }
}
