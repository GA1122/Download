SQLRETURN SQLGetDiagRecW( SQLSMALLINT handle_type,
        SQLHANDLE   handle,
        SQLSMALLINT rec_number,
        SQLWCHAR     *sqlstate,
        SQLINTEGER  *native,
        SQLWCHAR     *message_text,
        SQLSMALLINT buffer_length,
        SQLSMALLINT *text_length_ptr )
{
    SQLRETURN ret;
    SQLCHAR s0[ 32 ], s1[ 100 + LOG_MESSAGE_LEN ];
    SQLCHAR s2[ 100 + LOG_MESSAGE_LEN ];
    SQLCHAR s3[ 100 + LOG_MESSAGE_LEN ];

    if ( rec_number < 1 )
    {
        return SQL_ERROR;
    }

    if ( handle_type == SQL_HANDLE_ENV )
    {
        DMHENV environment = ( DMHENV ) handle;

        if ( !__validate_env( environment ))
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: SQL_INVALID_HANDLE" );

            return SQL_INVALID_HANDLE;
        }

        thread_protect( SQL_HANDLE_ENV, environment );

        if ( log_info.log_flag )
        {
            sprintf( environment -> msg, 
                "\n\t\tEntry:\
\n\t\t\tEnvironment = %p\
\n\t\t\tRec Number = %d\
\n\t\t\tSQLState = %p\
\n\t\t\tNative = %p\
\n\t\t\tMessage Text = %p\
\n\t\t\tBuffer Length = %d\
\n\t\t\tText Len Ptr = %p",
                    environment,
                    rec_number,
                    sqlstate,
                    native,
                    message_text,
                    buffer_length,
                    text_length_ptr );

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    environment -> msg );
        }

        ret = extract_sql_error_rec_w( &environment -> error,
                sqlstate,
                rec_number,
                native,
                message_text,
                buffer_length,
                text_length_ptr );

        if ( log_info.log_flag )
        {
            if ( SQL_SUCCEEDED( ret ))
            {
                char *ts1, *ts2;

                sprintf( environment -> msg, 
                    "\n\t\tExit:[%s]\
\n\t\t\tSQLState = %s\
\n\t\t\tNative = %s\
\n\t\t\tMessage Text = %s",
                        __get_return_status( ret, s2 ),
                        __sdata_as_string( s3, SQL_CHAR,
				NULL, ts1 = unicode_to_ansi_alloc( sqlstate, SQL_NTS, NULL, NULL  )),
                        __iptr_as_string( s0, native ),
                        __sdata_as_string( s1, SQL_CHAR, 
                            text_length_ptr, ts2 = unicode_to_ansi_alloc( message_text, SQL_NTS, NULL, NULL  )));

		if ( ts1 ) {
                	free( ts1 );
		}
		if ( ts2 ) {
                	free( ts2 );
		}
            }
            else
            {
                sprintf( environment -> msg, 
                    "\n\t\tExit:[%s]",
                        __get_return_status( ret, s2 ));
            }

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    environment -> msg );
        }

        thread_release( SQL_HANDLE_ENV, environment );

        return ret;
    }
    else if ( handle_type == SQL_HANDLE_DBC )
    {
        DMHDBC connection = ( DMHDBC ) handle;

        if ( !__validate_dbc( connection ))
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: SQL_INVALID_HANDLE" );

#ifdef WITH_HANDLE_REDIRECT
		{
			DMHDBC parent_connection;

			parent_connection = find_parent_handle( connection, SQL_HANDLE_DBC );

			if ( parent_connection ) {
        		dm_log_write( __FILE__, 
                	__LINE__, 
                    	LOG_INFO, 
                    	LOG_INFO, 
                    	"Info: found parent handle" );

				if ( CHECK_SQLGETDIAGRECW( parent_connection ))
				{
        			dm_log_write( __FILE__, 
                		__LINE__, 
                   		 	LOG_INFO, 
                   		 	LOG_INFO, 
                   		 	"Info: calling redirected driver function" );

					return SQLGETDIAGRECW( parent_connection, 
							handle_type,
							connection, 
        					rec_number,
        					sqlstate,
        					native,
        					message_text,
        					buffer_length,
        					text_length_ptr );
				}
			}
		}
#endif
            return SQL_INVALID_HANDLE;
        }

        thread_protect( SQL_HANDLE_DBC, connection );

        if ( log_info.log_flag )
        {
            sprintf( connection -> msg, 
                "\n\t\tEntry:\
\n\t\t\tConnection = %p\
\n\t\t\tRec Number = %d\
\n\t\t\tSQLState = %p\
\n\t\t\tNative = %p\
\n\t\t\tMessage Text = %p\
\n\t\t\tBuffer Length = %d\
\n\t\t\tText Len Ptr = %p",
                    connection,
                    rec_number,
                    sqlstate,
                    native,
                    message_text,
                    buffer_length,
                    text_length_ptr );

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    connection -> msg );
        }

        ret = extract_sql_error_rec_w( &connection -> error,
                sqlstate,
                rec_number,
                native,
                message_text,
                buffer_length,
                text_length_ptr );

        if ( log_info.log_flag )
        {
            if ( SQL_SUCCEEDED( ret ))
            {
                char *ts1, *ts2;

                sprintf( connection -> msg, 
                    "\n\t\tExit:[%s]\
\n\t\t\tSQLState = %s\
\n\t\t\tNative = %s\
\n\t\t\tMessage Text = %s",
                        __get_return_status( ret, s2 ),
                        __sdata_as_string( s3, SQL_CHAR,
				NULL, ts1 = unicode_to_ansi_alloc( sqlstate, SQL_NTS, connection, NULL  )),
                        __iptr_as_string( s0, native ),
                        __sdata_as_string( s1, SQL_CHAR, 
                            	text_length_ptr, ts2 = unicode_to_ansi_alloc( message_text, SQL_NTS, connection, NULL  )));

		if ( ts1 ) {
                	free( ts1 );
		}
		if ( ts2 ) {
                	free( ts2 );
		}
            }
            else
            {
                sprintf( connection -> msg, 
                    "\n\t\tExit:[%s]",
                        __get_return_status( ret, s2 ));
            }

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    connection -> msg );
        }

        thread_release( SQL_HANDLE_DBC, connection );

        return ret;
    }
    else if ( handle_type == SQL_HANDLE_STMT )
    {
        DMHSTMT statement = ( DMHSTMT ) handle;

        if ( !__validate_stmt( statement ))
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: SQL_INVALID_HANDLE" );

#ifdef WITH_HANDLE_REDIRECT
		{
			DMHSTMT parent_statement;

			parent_statement = find_parent_handle( statement, SQL_HANDLE_STMT );

			if ( parent_statement ) {
        		dm_log_write( __FILE__, 
                	__LINE__, 
                    	LOG_INFO, 
                    	LOG_INFO, 
                    	"Info: found parent handle" );

				if ( CHECK_SQLGETDIAGRECW( parent_statement -> connection ))
				{
        			dm_log_write( __FILE__, 
                		__LINE__, 
                   		 	LOG_INFO, 
                   		 	LOG_INFO, 
                   		 	"Info: calling redirected driver function" );

					return SQLGETDIAGRECW( parent_statement -> connection, 
							handle_type,
							statement,
        					rec_number,
        					sqlstate,
        					native,
        					message_text,
        					buffer_length,
        					text_length_ptr );
				}
			}
		}
#endif
            return SQL_INVALID_HANDLE;
        }

        thread_protect( SQL_HANDLE_STMT, statement );

        if ( log_info.log_flag )
        {
            sprintf( statement -> msg, 
                "\n\t\tEntry:\
\n\t\t\tStatement = %p\
\n\t\t\tRec Number = %d\
\n\t\t\tSQLState = %p\
\n\t\t\tNative = %p\
\n\t\t\tMessage Text = %p\
\n\t\t\tBuffer Length = %d\
\n\t\t\tText Len Ptr = %p",
                    statement,
                    rec_number,
                    sqlstate,
                    native,
                    message_text,
                    buffer_length,
                    text_length_ptr );

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    statement -> msg );
        }

        ret = extract_sql_error_rec_w( &statement -> error,
                sqlstate,
                rec_number,
                native,
                message_text,
                buffer_length,
                text_length_ptr );

        if ( log_info.log_flag )
        {
            if ( SQL_SUCCEEDED( ret ))
            {
                char *ts1, *ts2;

                sprintf( statement -> msg, 
                    "\n\t\tExit:[%s]\
\n\t\t\tSQLState = %s\
\n\t\t\tNative = %s\
\n\t\t\tMessage Text = %s",
                        __get_return_status( ret, s2 ),
                        __sdata_as_string( s3, SQL_CHAR,
				NULL, ts1 = unicode_to_ansi_alloc( sqlstate, SQL_NTS, statement -> connection, NULL  )),
                        __iptr_as_string( s0, native ),
                        __sdata_as_string( s1, SQL_CHAR, 
                            text_length_ptr, ts2 = unicode_to_ansi_alloc( message_text, SQL_NTS, statement -> connection, NULL  )));

		if ( ts1 ) {
                	free( ts1 );
		}
		if ( ts2 ) {
                	free( ts2 );
		}
            }
            else
            {
                sprintf( statement -> msg, 
                    "\n\t\tExit:[%s]",
                        __get_return_status( ret, s2 ));
            }

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    statement -> msg );
        }

        thread_release( SQL_HANDLE_STMT, statement );

        return ret;
    }
    else if ( handle_type == SQL_HANDLE_DESC )
    {
        DMHDESC descriptor = ( DMHDESC ) handle;

        if ( !__validate_desc( descriptor ))
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: SQL_INVALID_HANDLE" );

#ifdef WITH_HANDLE_REDIRECT
		{
			DMHDESC parent_desc;

			parent_desc = find_parent_handle( descriptor, SQL_HANDLE_DESC );

			if ( parent_desc ) {
        		dm_log_write( __FILE__, 
                	__LINE__, 
                    	LOG_INFO, 
                    	LOG_INFO, 
                    	"Info: found parent handle" );

				if ( CHECK_SQLGETDIAGRECW( parent_desc -> connection ))
				{
        			dm_log_write( __FILE__, 
                		__LINE__, 
                   		 	LOG_INFO, 
                   		 	LOG_INFO, 
                   		 	"Info: calling redirected driver function" );

					return SQLGETDIAGRECW( parent_desc -> connection, 
							handle_type,
							descriptor,
        					rec_number,
        					sqlstate,
        					native,
        					message_text,
        					buffer_length,
        					text_length_ptr );
				}
			}
		}
#endif
            return SQL_INVALID_HANDLE;
        }

        thread_protect( SQL_HANDLE_DESC, descriptor );

        if ( log_info.log_flag )
        {
            sprintf( descriptor -> msg, 
                "\n\t\tEntry:\
\n\t\t\tDescriptor = %p\
\n\t\t\tRec Number = %d\
\n\t\t\tSQLState = %p\
\n\t\t\tNative = %p\
\n\t\t\tMessage Text = %p\
\n\t\t\tBuffer Length = %d\
\n\t\t\tText Len Ptr = %p",
                    descriptor,
                    rec_number,
                    sqlstate,
                    native,
                    message_text,
                    buffer_length,
                    text_length_ptr );

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    descriptor -> msg );
        }

        ret = extract_sql_error_rec_w( &descriptor -> error,
                sqlstate,
                rec_number,
                native,
                message_text,
                buffer_length,
                text_length_ptr );

        if ( log_info.log_flag )
        {
            if ( SQL_SUCCEEDED( ret ))
            {
                char *ts1, *ts2;

                sprintf( descriptor -> msg, 
                    "\n\t\tExit:[%s]\
\n\t\t\tSQLState = %s\
\n\t\t\tNative = %s\
\n\t\t\tMessage Text = %s",
                        __get_return_status( ret, s2 ),
                        __sdata_as_string( s3, SQL_CHAR,
				NULL, ts1 = unicode_to_ansi_alloc( sqlstate, SQL_NTS, descriptor -> connection, NULL  )),
                        __iptr_as_string( s0, native ),
                        __sdata_as_string( s1, SQL_CHAR, 
                            text_length_ptr, ts2 = unicode_to_ansi_alloc( message_text, SQL_NTS, descriptor -> connection, NULL  )));

		if ( ts1 ) {
                	free( ts1 );
		}
		if ( ts2 ) {
                	free( ts2 );
		}
            }
            else
            {
                sprintf( descriptor -> msg, 
                    "\n\t\tExit:[%s]",
                        __get_return_status( ret, s2 ));
            }

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    descriptor -> msg );
        }

        thread_release( SQL_HANDLE_DESC, descriptor );

        return ret;
    }
    return SQL_NO_DATA;
}
