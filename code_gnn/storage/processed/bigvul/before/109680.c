FormController* Document::formController()
{
    if (!m_formController)
        m_formController = FormController::create();
    return m_formController.get();
}
