#ifndef STATIONMODEL_H
#define STATIONMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QGeoCoordinate>
/*-------------------------------GPL-------------------------------------//
//
// MetOcean Viewer - A simple interface for viewing hydrodynamic model data
// Copyright (C) 2015  Zach Cobell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------*/
#include <QHash>
#include <QObject>
#include "station.h"

class StationModel : public QAbstractListModel {
  Q_OBJECT

 public:
  using QAbstractListModel::QAbstractListModel;

  enum StationRoles {
    positionRole = Qt::UserRole + 1,
    stationIDRole,
    stationNameRole,
    longitudeRole,
    latitudeRole,
    valueRole
  };

  StationModel(QObject *parent = Q_NULLPTR);

  Q_INVOKABLE void addMarker(const Station &station);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const;

  Station findStation(QString stationName) const;

 private:
  QList<Station> m_stations;
  QHash<QString, Station> m_stationMap;
};

#endif  // STATIONMODEL_H
