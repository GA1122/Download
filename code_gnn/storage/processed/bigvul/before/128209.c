NavigatorVibration::VibrationPattern Notification::vibrate(bool& isNull) const
{
    NavigatorVibration::VibrationPattern pattern;
    pattern.appendRange(m_data.vibrate.begin(), m_data.vibrate.end());

    if (!pattern.size())
        isNull = true;

    return pattern;
}
