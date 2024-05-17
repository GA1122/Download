CACHE_LIMITER_FUNC(nocache)  
{
	ADD_HEADER("Expires: Thu, 19 Nov 1981 08:52:00 GMT");

	 
	ADD_HEADER("Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0");

	 
	ADD_HEADER("Pragma: no-cache");
}
 
