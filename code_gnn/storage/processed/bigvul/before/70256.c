Luv24toXYZ(LogLuvState* sp, uint8* op, tmsize_t n)
{
	uint32* luv = (uint32*) sp->tbuf;  
	float* xyz = (float*) op;

	while (n-- > 0) {
		LogLuv24toXYZ(*luv, xyz);
		xyz += 3;
		luv++;
	}
}