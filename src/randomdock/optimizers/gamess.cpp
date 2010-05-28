/**********************************************************************
  GAMESSOptimizer - Tools to interface with GAMESS remotely

  Copyright (C) 2009 by David C. Lonie

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.openmolecules.net/>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#include "gamess.h"
#include "../../generic/templates.h"

#include <QProcess>
#include <QDir>
#include <QString>
#include <QDebug>

#include "../../generic/structure.h"

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

using namespace std;
using namespace OpenBabel;
using namespace Eigen;

namespace Avogadro {

  GAMESSOptimizer::GAMESSOptimizer(OptBase *parent) :
    Optimizer(parent)
  {
    // Set allowed data structure keys, if any, e.g.
    // None here!

    // Set allowed filenames, e.g.
    m_templates.insert("job.inp",QStringList(""));
    m_templates.insert("job.pbs",QStringList(""));

    // Setup for completion values
    m_completionFilename = "job.gamout";
    m_completionString   = "***** EQUILIBRIUM GEOMETRY LOCATED *****";

    // Set output filenames to try to read data from
    m_outputFilenames.append(m_completionFilename);

    // Set the name of the optimizer to be returned by getIDString()
    m_idString = "GAMESS";

    readSettings();
  }

} // end namespace Avogadro

//#include "gamess.moc"
