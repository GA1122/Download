 static SQLRETURN extract_sql_error_rec_w( EHEAD *head,
         SQLWCHAR *sqlstate,
         SQLINTEGER rec_number,
        SQLINTEGER *native_error,
        SQLWCHAR *message_text,
        SQLSMALLINT buffer_length,
        SQLSMALLINT *text_length )
{
    SQLRETURN ret;

    if ( sqlstate )
    {
        SQLWCHAR *tmp;

        tmp = ansi_to_unicode_alloc((SQLCHAR*) "00000", SQL_NTS, __get_connection( head ), NULL );
        wide_strcpy( sqlstate, tmp );
        free( tmp );
    }

    if ( rec_number <= head -> sql_diag_head.internal_count )
    {
        ERROR *ptr;

        ptr = head -> sql_diag_head.internal_list_head;
        while( rec_number > 1 )
        {
            ptr = ptr -> next;
            rec_number --;
        }

		if ( !ptr ) 
		{
	    	return SQL_NO_DATA;
		}

        if ( sqlstate )
        {
            wide_strcpy( sqlstate, ptr -> sqlstate );
        }
        if ( buffer_length < wide_strlen( ptr -> msg ) + 1 )
        {
            ret = SQL_SUCCESS_WITH_INFO;
        }
        else
        {
            ret = SQL_SUCCESS;
        }

        if ( message_text )
        {
            if ( ret == SQL_SUCCESS )
            {
                wide_strcpy( message_text, ptr -> msg );
            }
            else
            {
                memcpy( message_text, ptr -> msg, buffer_length * 2 );
                message_text[ buffer_length - 1 ] = '\0';
            }
        }

        if ( text_length )
        {
            *text_length = wide_strlen( ptr -> msg );
        }

        if ( native_error )
        {
            *native_error = ptr -> native_error;
        }

         

        if ( SQL_SUCCEEDED( ret ) && sqlstate )
            __map_error_state_w(sqlstate, __get_version( head ));

        return ret;
    }
    else if ( !__is_env( head ) && __get_connection( head ) -> state != STATE_C2
        && head->sql_diag_head.error_count )
    {
        ERROR *ptr;
        rec_number -= head -> sql_diag_head.internal_count;

        if ( __get_connection( head ) -> unicode_driver &&
            CHECK_SQLGETDIAGRECW( __get_connection( head )))
        {
            ret = SQLGETDIAGRECW( __get_connection( head ),
                    head -> handle_type,
                    __get_driver_handle( head ),
                    rec_number,
                    sqlstate,
                    native_error,
                    message_text,
                    buffer_length,
                    text_length );

             

            if ( SQL_SUCCEEDED( ret ) && sqlstate )
            {
                __map_error_state_w( sqlstate, __get_version( head ));
            }

            return ret;
        }
        else if ( !__get_connection( head ) -> unicode_driver &&
            CHECK_SQLGETDIAGREC( __get_connection( head )))
        {
            SQLCHAR *as1 = NULL, *as2 = NULL;

            if ( sqlstate )
            {
                as1 = malloc( 7 );
            }

            if ( message_text && buffer_length > 0 )
            {
                as2 = malloc( buffer_length + 1 );
            }
                
            ret = SQLGETDIAGREC( __get_connection( head ),
                    head -> handle_type,
                    __get_driver_handle( head ),
                    rec_number,
                    as1 ? as1 : (SQLCHAR *)sqlstate,
                    native_error,
                    as2 ? as2 : (SQLCHAR *)message_text,
                    buffer_length,
                    text_length );

             

            if ( SQL_SUCCEEDED( ret ) && sqlstate )
            {
                if ( sqlstate )
                {
                    if ( as1 )
                    {
                        ansi_to_unicode_copy( sqlstate,(char*) as1, SQL_NTS, __get_connection( head ), NULL );
                        __map_error_state_w( sqlstate, __get_version( head ));
                    }
                }
                if ( message_text )
                {
                    if ( as2 )
                    {
                        ansi_to_unicode_copy( message_text,(char*) as2, SQL_NTS, __get_connection( head ), NULL );
                    }
                }
            }

            if ( as1 ) free( as1 );
            if ( as2 ) free( as2 );

            return ret;
        }
        else
        {
            ptr = head -> sql_diag_head.error_list_head;
            while( rec_number > 1 )
            {
                ptr = ptr -> next;
                rec_number --;
            }

			if ( !ptr ) 
			{
	    		return SQL_NO_DATA;
			}

            if ( sqlstate )
            {
                wide_strcpy( sqlstate, ptr -> sqlstate );
            }
            if ( buffer_length < wide_strlen( ptr -> msg ) + 1 )
            {
                ret = SQL_SUCCESS_WITH_INFO;
            }
            else
            {
                ret = SQL_SUCCESS;
            }

            if ( message_text )
            {
                if ( ret == SQL_SUCCESS )
                {
                    wide_strcpy( message_text, ptr -> msg );
                }
                else
                {
                    memcpy( message_text, ptr -> msg, buffer_length * 2 );
                    message_text[ buffer_length - 1 ] = '\0';
                }
            }

            if ( text_length )
            {
                *text_length = wide_strlen( ptr -> msg );
            }

            if ( native_error )
            {
                *native_error = ptr -> native_error;
            }

             

            if ( SQL_SUCCEEDED( ret ) && sqlstate )
                __map_error_state_w( sqlstate, __get_version( head ));

            return ret;
        }
    }
    else
    {
        return SQL_NO_DATA;
    }
}
