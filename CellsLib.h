#ifndef NETLIST_CELLS_LIBS_H
#define NETLIST_CELLS_LIBS_H

#include "CellsModel.h"
#include "CellViewer.h"
#include <QTableView>
#include <QPushButton>

namespace Netlist {
	
	class CellViewer;

	class CellsLib : public QWidget {
	    Q_OBJECT;
	  public:
		               ~CellsLib();
			       CellsLib       ( QWidget* parent=NULL );
	    inline void        setCellViewer  ( CellViewer* );
		   int         getSelectedRow () const;
	    inline CellsModel* getBaseModel   ();
	  public slots:
		   void        load           ();
	  private:
	    CellViewer*  cellViewer_;
	    CellsModel*  baseModel_;
	    QTableView*  view_;
	    QPushButton* load_;
	};

	inline CellsModel* CellsLib::getBaseModel() {
		return baseModel_;
	}

	inline void CellsLib::setCellViewer(CellViewer* c) {
		cellViewer_ = c;
	}
}


#endif
