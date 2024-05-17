bdfReadFontInfo(FontInfoPtr pFontInfo, FontFilePtr file)
{
    FontRec     font;
    int         ret;

    bzero(&font, sizeof (FontRec));

    ret = bdfReadFont(&font, file, MSBFirst, LSBFirst, 1, 1);
    if (ret == Successful) {
	*pFontInfo = font.info;
	font.info.props = 0;
	font.info.isStringProp = 0;
	font.info.nprops = 0;
	bdfFreeFontBits (&font);
    }
    return ret;
}
