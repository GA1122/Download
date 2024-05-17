void Part::slotShowProperties()
{
    m_model->countEntriesAndSize();
    QPointer<Kerfuffle::PropertiesDialog> dialog(new Kerfuffle::PropertiesDialog(0,
                                                                                 m_model->archive(),
                                                                                 m_model->numberOfFiles(),
                                                                                 m_model->numberOfFolders(),
                                                                                 m_model->uncompressedSize()));
    dialog.data()->show();
}
