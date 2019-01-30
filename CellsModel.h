#ifndef NETLIST_CELLS_MODEL_H
#define NETLIST_CELLS_MODEL_H

#include  <QAbstractTableModel>
#include  <Cell.h>

namespace Netlist {
	class CellsModel : public QAbstractTableModel{
	Q_OBJECT;

		public:
			CellsModel ( QObject* parent=NULL );
			~CellsModel  ();
			Cell*       getModel(int row);
			int          rowCount         ( const  QModelIndex& parent=QModelIndex () ) const;
			int          columnCount     ( const  QModelIndex& parent=QModelIndex () ) const;
			QVariant    data             ( const  QModelIndex& index , int  role=Qt:: DisplayRole ) const;
			QVariant    headerData      ( int  section, Qt:: Orientation  orientation, int  role=Qt:: DisplayRole ) const;
		public slots:
			void 	    updateDatas();
	};
}

#endif
