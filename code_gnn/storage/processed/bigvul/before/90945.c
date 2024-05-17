float pressureToAltitude(float seaLevel, float atmospheric, float temp)
{
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	return (((float)pow((seaLevel / atmospheric), 0.190223F) - 1.0F)
		* (temp + 273.15F)) / 0.0065F;
}
