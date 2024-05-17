    static bool MapStringToTime(base::StringPiece value, base::Time* time) {
      return base::Time::FromString(value.as_string().c_str(), time);
    }
