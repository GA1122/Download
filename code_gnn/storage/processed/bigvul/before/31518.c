ZEND_GET_MODULE(radius)
#endif

 
PHP_MINIT_FUNCTION(radius)
{
	le_radius = zend_register_list_destructors_ex(_radius_close, NULL, "rad_handle", module_number);
#include "radius_init_const.h"
	REGISTER_LONG_CONSTANT("RADIUS_MPPE_KEY_LEN", MPPE_KEY_LEN, CONST_PERSISTENT);    
	return SUCCESS;
}
 