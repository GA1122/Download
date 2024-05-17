void WT_UpdateLFO (S_LFO_CONTROL *pLFO, EAS_I16 phaseInc)
{

  
 if (pLFO->lfoPhase < 0)
 {
        pLFO->lfoPhase++;
 return;
 }

  
  
    pLFO->lfoValue = (EAS_I16) (pLFO->lfoPhase << 2);
  
 if ((pLFO->lfoPhase > 0x1fff) && (pLFO->lfoPhase < 0x6000))
        pLFO->lfoValue = ~pLFO->lfoValue;

  
    pLFO->lfoPhase = (pLFO->lfoPhase + phaseInc) & 0x7fff;
}
