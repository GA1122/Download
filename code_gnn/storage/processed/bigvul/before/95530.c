static qboolean S_AL_HearingThroughEntity( int entityNum )
{
	float	distanceSq;

	if( lastListenerNumber == entityNum )
	{
		distanceSq = DistanceSquared(
				entityList[ entityNum ].origin,
				lastListenerOrigin );

		if( distanceSq > THIRD_PERSON_THRESHOLD_SQ )
			return qfalse;  
		else
			return qtrue;   
	}
	else
		return qfalse;  
}
