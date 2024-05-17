    void writeNumberObject(double number)
    {
        append(NumberObjectTag);
        doWriteNumber(number);
    }
