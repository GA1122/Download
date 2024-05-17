    void kick(int misspellingStartOffset, int misspellingLength, WebTextDecorationType type)
    {
        if (!m_completion)
            return;
        Vector<WebTextCheckingResult> results;
        if (misspellingStartOffset >= 0 && misspellingLength > 0)
            results.append(WebTextCheckingResult(type, misspellingStartOffset, misspellingLength));
        m_completion->didFinishCheckingText(results);
        m_completion = 0;
    }
