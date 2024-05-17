rcFile(char *base)
{
    if (base &&
	(base[0] == '/' ||
	 (base[0] == '.'
	  && (base[1] == '/' || (base[1] == '.' && base[2] == '/')))
	 || (base[0] == '~' && base[1] == '/')))
	 
	return expandPath(base);
    return expandPath(Strnew_m_charp(rc_dir, "/", base, NULL)->ptr);
}