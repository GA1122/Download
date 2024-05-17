static UINT16 GetDigestSize( TPM_ALG_ID authHash )
{
    UINT32 i;
    for(i = 0; i < (sizeof(hashSizes)/sizeof(HASH_SIZE_INFO)); i++ )
    {
        if( hashSizes[i].algId == authHash )
            return hashSizes[i].size;
    }

    return( 0 );
}
