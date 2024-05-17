php_mysqlnd_read_row_ex(MYSQLND_CONN_DATA * conn, MYSQLND_MEMORY_POOL * result_set_memory_pool,
						MYSQLND_MEMORY_POOL_CHUNK ** buffer,
						size_t * data_size, zend_bool persistent_alloc,
						unsigned int prealloc_more_bytes TSRMLS_DC)
{
	enum_func_status ret = PASS;
	MYSQLND_PACKET_HEADER header;
	zend_uchar * p = NULL;
	zend_bool first_iteration = TRUE;

	DBG_ENTER("php_mysqlnd_read_row_ex");

	 

	*data_size = prealloc_more_bytes;
	while (1) {
		if (FAIL == mysqlnd_read_header(conn->net, &header, conn->stats, conn->error_info TSRMLS_CC)) {
			ret = FAIL;
			break;
		}

		*data_size += header.size;

		if (first_iteration) {
			first_iteration = FALSE;
			 
			*buffer = result_set_memory_pool->get_chunk(result_set_memory_pool, *data_size + 1 TSRMLS_CC);
			if (!*buffer) {
				ret = FAIL;
				break;
			}
			p = (*buffer)->ptr;
		} else if (!first_iteration) {
			 
			if (!header.size) {
				break;
			}

			 
			if (FAIL == (*buffer)->resize_chunk((*buffer), *data_size + 1 TSRMLS_CC)) {
				SET_OOM_ERROR(*conn->error_info);
				ret = FAIL;
				break;
			}
			 
			p = (*buffer)->ptr + (*data_size - header.size);
		}

		if (PASS != (ret = conn->net->data->m.receive_ex(conn->net, p, header.size, conn->stats, conn->error_info TSRMLS_CC))) {
			DBG_ERR("Empty row packet body");
			php_error(E_WARNING, "Empty row packet body");
			break;
		}

		if (header.size < MYSQLND_MAX_PACKET_SIZE) {
			break;
		}
	}
	if (ret == FAIL && *buffer) {
		(*buffer)->free_chunk((*buffer) TSRMLS_CC);
		*buffer = NULL;
	}
	*data_size -= prealloc_more_bytes;
	DBG_RETURN(ret);
}
