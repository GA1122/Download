int mbedtls_ecp_check_pub_priv( const mbedtls_ecp_keypair *pub, const mbedtls_ecp_keypair *prv )
{
    int ret;
    mbedtls_ecp_point Q;
    mbedtls_ecp_group grp;

    if( pub->grp.id == MBEDTLS_ECP_DP_NONE ||
        pub->grp.id != prv->grp.id ||
        mbedtls_mpi_cmp_mpi( &pub->Q.X, &prv->Q.X ) ||
        mbedtls_mpi_cmp_mpi( &pub->Q.Y, &prv->Q.Y ) ||
        mbedtls_mpi_cmp_mpi( &pub->Q.Z, &prv->Q.Z ) )
    {
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    }

    mbedtls_ecp_point_init( &Q );
    mbedtls_ecp_group_init( &grp );

     
    mbedtls_ecp_group_copy( &grp, &prv->grp );

     
    MBEDTLS_MPI_CHK( mbedtls_ecp_mul( &grp, &Q, &prv->d, &prv->grp.G, NULL, NULL ) );

    if( mbedtls_mpi_cmp_mpi( &Q.X, &prv->Q.X ) ||
        mbedtls_mpi_cmp_mpi( &Q.Y, &prv->Q.Y ) ||
        mbedtls_mpi_cmp_mpi( &Q.Z, &prv->Q.Z ) )
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free( &Q );
    mbedtls_ecp_group_free( &grp );

    return( ret );
}