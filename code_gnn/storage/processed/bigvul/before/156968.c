 double WebMediaPlayerMS::MediaTimeForTimeValue(double timeValue) const {
   return base::TimeDelta::FromSecondsD(timeValue).InSecondsF();
 }
