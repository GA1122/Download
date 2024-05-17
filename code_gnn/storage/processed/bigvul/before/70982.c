void *Type_Curve_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag)
{
    cmsUInt32Number Count;
    cmsToneCurve* NewGamma;
    
    *nItems = 0;
    if (!_cmsReadUInt32Number(io, &Count)) return NULL;

    switch (Count) {

           case 0:    
               {
                   cmsFloat64Number SingleGamma = 1.0;

                   NewGamma = cmsBuildParametricToneCurve(self ->ContextID, 1, &SingleGamma);
                   if (!NewGamma) return NULL;
                   *nItems = 1;
                   return NewGamma;
               }
              
           case 1:   
               {
                   cmsUInt16Number SingleGammaFixed;
                   cmsFloat64Number SingleGamma;

                   if (!_cmsReadUInt16Number(io, &SingleGammaFixed)) return NULL;
                   SingleGamma = _cms8Fixed8toDouble(SingleGammaFixed);

                   *nItems = 1;
                   return cmsBuildParametricToneCurve(self ->ContextID, 1, &SingleGamma);
               }

           default:   

               if (Count > 0x7FFF)
                   return NULL;  

               NewGamma = cmsBuildTabulatedToneCurve16(self ->ContextID, Count, NULL);
               if (!NewGamma) return NULL;

               if (!_cmsReadUInt16Array(io, Count, NewGamma -> Table16)) return NULL;

               *nItems = 1;
               return NewGamma;
    }

    cmsUNUSED_PARAMETER(SizeOfTag);
}
