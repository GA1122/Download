 static PHP_RINIT_FUNCTION(libxml)
 {
 	if (_php_libxml_per_request_initialization) {
 		 
                 xmlSetGenericErrorFunc(NULL, php_libxml_error_handler);
                 xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_create_filename);
                 xmlOutputBufferCreateFilenameDefault(php_libxml_output_buffer_create_filename);
// 
//                 
//                LIBXML(entity_loader_disabled) = 0;
         }
         return SUCCESS;
  }