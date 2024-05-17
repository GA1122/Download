void Part::slotCutFiles()
{
    QModelIndexList selectedRows = addChildren(m_view->selectionModel()->selectedRows());
    m_model->filesToMove = ArchiveModel::entryMap(filesForIndexes(selectedRows));
    qCDebug(ARK) << "Entries marked to cut:" << m_model->filesToMove.values();
    m_model->filesToCopy.clear();
    foreach (const QModelIndex &row, m_cutIndexes) {
        m_view->dataChanged(row, row);
    }
    m_cutIndexes = selectedRows;
    foreach (const QModelIndex &row, m_cutIndexes) {
        m_view->dataChanged(row, row);
    }
    updateActions();
}
