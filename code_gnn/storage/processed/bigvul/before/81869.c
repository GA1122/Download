static void wc_ecc_dump_oids(void)
{
    int x;

    if (mOidDumpDone) {
        return;
    }

     
    for (x = 0; ecc_sets[x].size != 0; x++) {
        int i;
        byte* oid;
        word32 oidSz, sum = 0;

        printf("ECC %s (%d):\n", ecc_sets[x].name, x);

    #ifdef HAVE_OID_ENCODING
        byte oidEnc[ECC_MAX_OID_LEN];

        oid = oidEnc;
        oidSz = ECC_MAX_OID_LEN;

        printf("OID: ");
        for (i = 0; i < (int)ecc_sets[x].oidSz; i++) {
            printf("%d.", ecc_sets[x].oid[i]);
        }
        printf("\n");

        EncodeObjectId(ecc_sets[x].oid, ecc_sets[x].oidSz, oidEnc, &oidSz);
    #else
        oid = (byte*)ecc_sets[x].oid;
        oidSz = ecc_sets[x].oidSz;
    #endif

        printf("OID Encoded: ");
        for (i = 0; i < (int)oidSz; i++) {
            printf("0x%02X,", oid[i]);
        }
        printf("\n");

        for (i = 0; i < (int)oidSz; i++) {
            sum += oid[i];
        }
        printf("Sum: %d\n", sum);

         
        if (ecc_sets[x].oidSum != sum) {
            printf("  Sum %d Not Valid!\n", ecc_sets[x].oidSum);
        }
    }
    mOidDumpDone = 1;
}