static inline uint32_t clipf_c_one(uint32_t a, uint32_t mini,
                   uint32_t maxi, uint32_t maxisign)
{

    if(a > mini) return mini;
    else if((a^(1U<<31)) > maxisign) return maxi;
    else return a;
}
