void CL_UpdateMumble(void)
{
	vec3_t pos, forward, up;
	float scale = cl_mumbleScale->value;
	float tmp;
	
	if(!cl_useMumble->integer)
		return;

	AngleVectors( cl.snap.ps.viewangles, forward, NULL, up);

	pos[0] = cl.snap.ps.origin[0] * scale;
	pos[1] = cl.snap.ps.origin[2] * scale;
	pos[2] = cl.snap.ps.origin[1] * scale;

	tmp = forward[1];
	forward[1] = forward[2];
	forward[2] = tmp;

	tmp = up[1];
	up[1] = up[2];
	up[2] = tmp;

	if(cl_useMumble->integer > 1) {
		fprintf(stderr, "%f %f %f, %f %f %f, %f %f %f\n",
			pos[0], pos[1], pos[2],
			forward[0], forward[1], forward[2],
			up[0], up[1], up[2]);
	}

	mumble_update_coordinates(pos, forward, up);
}
