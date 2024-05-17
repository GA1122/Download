static int uvesafb_pan_display(struct fb_var_screeninfo *var,
		struct fb_info *info)
{
#ifdef CONFIG_X86_32
	int offset;
	struct uvesafb_par *par = info->par;

	offset = (var->yoffset * info->fix.line_length + var->xoffset) / 4;

	 
	if (par->pmi_start) {
		__asm__ __volatile__(
			"call *(%%edi)"
			:  
			: "a" (0x4f07),          
			  "b" (0),               
			  "c" (offset),          
			  "d" (offset >> 16),    
			  "D" (&par->pmi_start));     
	}
#endif
	return 0;
}
