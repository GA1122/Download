static int x509_name_cmp( const mbedtls_x509_name *a, const mbedtls_x509_name *b )
{
     
    while( a != NULL || b != NULL )
    {
        if( a == NULL || b == NULL )
            return( -1 );

         
        if( a->oid.tag != b->oid.tag ||
            a->oid.len != b->oid.len ||
            memcmp( a->oid.p, b->oid.p, b->oid.len ) != 0 )
        {
            return( -1 );
        }

         
        if( x509_string_cmp( &a->val, &b->val ) != 0 )
            return( -1 );

         
        if( a->next_merged != b->next_merged )
            return( -1 );

        a = a->next;
        b = b->next;
    }

     
    return( 0 );
}
