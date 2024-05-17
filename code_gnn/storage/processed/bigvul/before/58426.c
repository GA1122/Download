cmsBool IsEmptyLayer(cmsMAT3* m, cmsVEC3* off)
{
    cmsFloat64Number diff = 0;
    cmsMAT3 Ident;
    int i;

    if (m == NULL && off == NULL) return TRUE;   
    if (m == NULL && off != NULL) return FALSE;  

    _cmsMAT3identity(&Ident);

    for (i=0; i < 3*3; i++)
        diff += fabs(((cmsFloat64Number*)m)[i] - ((cmsFloat64Number*)&Ident)[i]);

    for (i=0; i < 3; i++)
        diff += fabs(((cmsFloat64Number*)off)[i]);


    return (diff < 0.002);
}