/**********************************************************************
  RandomDock -- A tool for analysing a matrix-substrate docking problem

  Copyright (C) 2009 by David Lonie

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#include "tab_init.h"

#include "randomdock.h"
#include "randomdockdialog.h"
#include "substratemol.h"
#include "matrixmol.h"

#include <avogadro/moleculefile.h>

#include <QSettings>
#include <QFileDialog>

using namespace std;

namespace Avogadro {

  TabInit::TabInit( RandomDockParams *p ) :
    QObject( p->dialog ), m_params(p)
  {
    qDebug() << "TabInit::TabInit( " << p <<  " ) called.";

    m_tab_widget = new QWidget;
    ui.setupUi(m_tab_widget);

    // dialog connections
    connect(p->dialog, SIGNAL(tabsReadSettings()),
            this, SLOT(readSettings()));
    connect(p->dialog, SIGNAL(tabsWriteSettings()),
            this, SLOT(writeSettings()));

    // tab connections
    connect(ui.edit_substrateFile, SIGNAL(textChanged(QString)),
            this, SLOT(updateParams()));
    connect(ui.push_substrateBrowse, SIGNAL(clicked()),
            this, SLOT(substrateBrowse()));
    connect(ui.push_substrateCurrent, SIGNAL(clicked()),
            this, SLOT(substrateCurrent()));
    connect(ui.push_matrixAdd, SIGNAL(clicked()),
            this, SLOT(matrixAdd()));
    connect(ui.push_matrixRemove, SIGNAL(clicked()),
            this, SLOT(matrixRemove()));
    connect(ui.push_matrixCurrent, SIGNAL(clicked()),
            this, SLOT(matrixCurrent()));
    connect(ui.table_matrix, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(updateParams()));
    connect(ui.push_readFiles, SIGNAL(clicked()),
            this, SLOT(readFiles()));
  }

  TabInit::~TabInit()
  {
    qDebug() << "TabInit::~TabInit() called";
    writeSettings();
  }

  void TabInit::writeSettings() {
    qDebug() << "TabInit::writeSettings() called";
    QSettings settings; // Already set up in avogadro/src/main.cpp
  }

  void TabInit::readSettings() {
    qDebug() << "TabInit::readSettings() called";
    QSettings settings; // Already set up in avogadro/src/main.cpp
  }

  void TabInit::updateParams() {
    qDebug() << "TabInit::updateParams() called";
    m_params->substrateFile = ui.edit_substrateFile->text();
    m_params->matrixFiles.clear();
    m_params->matrixStoich.clear();
    for (int i = 0; i < ui.table_matrix->rowCount(); i++) {
      ui.table_matrix->item(i, Num)->setText(QString::number(i+1));
      m_params->matrixFiles.append(ui.table_matrix->item(i, Filename)->text());
      m_params->matrixStoich.append(ui.table_matrix->item(i, Stoich)->text().toInt());
    }
  }

  void TabInit::substrateBrowse() {
    qDebug() << "TabInit::substrateBrowse() called";
    QSettings settings;
    QString path = settings.value("randomdock/paths/moleculeBrowse", "").toString();
    QString fileName = QFileDialog::getOpenFileName(m_params->dialog, 
                                                    tr("Select molecule file to use for the substrate"),
                                                    path,
                                                    tr("All files (*)"));
    settings.setValue("randomdock/paths/moleculeBrowse", fileName);
    ui.edit_substrateFile->setText(fileName);
  }

  void TabInit::substrateCurrent() {
    qDebug() << "TabInit::substrateCurrent() called";
  }

  void TabInit::matrixAdd() {
    qDebug() << "TabInit::matrixAdd() called";
    QSettings settings;
    QString path = settings.value("randomdock/paths/moleculeBrowse", "").toString();
    QString fileName = QFileDialog::getOpenFileName(m_params->dialog, 
                                                    tr("Select molecule file to add as a matrix element"),
                                                    path,
                                                    tr("All files (*)"));
    settings.setValue("randomdock/paths/moleculeBrowse", fileName);

    int row = ui.table_matrix->rowCount();
    ui.table_matrix->insertRow(row);
    // Block signals for all but the last addition -- this keeps
    // updateParams() from crashing by not reading null items.
    ui.table_matrix->blockSignals(true);
    ui.table_matrix->setItem(row, Num, new QTableWidgetItem(QString::number(row+1)));
    ui.table_matrix->setItem(row, Filename, new QTableWidgetItem(fileName));
    ui.table_matrix->blockSignals(false);
    ui.table_matrix->setItem(row, Stoich, new QTableWidgetItem(QString::number(1)));
  }

  void TabInit::matrixRemove() {
    qDebug() << "TabInit::matrixRemove() called";
    ui.table_matrix->removeRow(ui.table_matrix->currentRow());
    updateParams();
  }

  void TabInit::matrixCurrent() {
    qDebug() << "TabInit::matrixCurrent() called";
  }

  void TabInit::readFiles() {
    qDebug() << "TabInit::readFiles() called";

    QApplication::setOverrideCursor( Qt::WaitCursor );
    Molecule *mol;

    // Substrate
    if (m_params->substrate) {
      m_params->substrate = 0;
    }
    qDebug() << m_params->substrateFile;
    if (!m_params->substrateFile.isEmpty()) {
      mol = MoleculeFile::readMolecule(m_params->substrateFile);
      m_params->substrate = new Substrate (mol);
      qDebug() << "Updated substrate: " << m_params->substrate << " #atoms= " << m_params->substrate->numAtoms();
    }

    // Matrix
    m_params->matrixList->clear();
    for (int i = 0; i < m_params->matrixFiles.size(); i++) {
      qDebug() << m_params->matrixFiles.at(i);
      mol = MoleculeFile::readMolecule(m_params->matrixFiles.at(i));
      m_params->matrixList->append(new Matrix (mol));
      qDebug() << "Matrix added:" << m_params->matrixList->at(i);
    }
    QApplication::restoreOverrideCursor();
  }

}

//#include "tab_init.moc"
