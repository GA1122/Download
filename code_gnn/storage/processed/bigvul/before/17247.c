CatalogueFreeFPE (FontPathElementPtr fpe)
{
    CataloguePtr	cat = fpe->private;

     

    if (!CatalogueNameCheck (fpe->name))
	return FontFileFreeFPE (fpe);

    CatalogueUnrefFPEs (fpe);
    xfree(cat->fpeList);
    xfree(cat);

    return Successful;
}
