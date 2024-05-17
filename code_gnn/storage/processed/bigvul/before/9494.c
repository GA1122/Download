static NOINLINE double my_SQRT(double X)
{
	union {
		float   f;
		int32_t i;
	} v;
	double invsqrt;
	double Xhalf = X * 0.5;

	 
	v.f = X;
	 
	v.i = 0x5f375a86 - (v.i >> 1);  
	invsqrt = v.f;  

	 
	invsqrt = invsqrt * (1.5 - Xhalf * invsqrt * invsqrt);  
	 
	 

	return X * invsqrt;  
}
