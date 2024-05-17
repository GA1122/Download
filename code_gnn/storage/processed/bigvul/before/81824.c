static INLINE void IncrementX963KdfCounter(byte* inOutCtr)
{
    int i;

     
    for (i = 3; i >= 0; i--) {
        if (++inOutCtr[i])   
            return;
    }
}
