/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_CORE_SHARED_EXPORT_H_
#define TANO_CORE_SHARED_EXPORT_H_

#include <QtCore/QtGlobal>

#if defined(TANO_CORE_LIBRARY)
#  define TANO_CORE_EXPORT Q_DECL_EXPORT
#else
#  define TANO_CORE_EXPORT Q_DECL_IMPORT
#endif

#endif // TANO_CORE_SHARED_EXPORT_H_
