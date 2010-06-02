/**********************************************************************
  XtalOptOptimizer - Generic optimizer interface

  Copyright (C) 2010 by David C. Lonie

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

#ifndef XTALOPTOPTIMIZER_H
#define XTALOPTOPTIMIZER_H

#include "../../generic/optimizer.h"

#include <QObject>

namespace Avogadro {
  class Structure;
  class OptBase;

  class XtalOptOptimizer : public Optimizer
  {
    Q_OBJECT

  public:

    explicit XtalOptOptimizer(OptBase *parent);
    virtual ~XtalOptOptimizer();

    virtual bool read(Structure *structure, const QString & filename);

  };

} // end namespace Avogadro

#endif