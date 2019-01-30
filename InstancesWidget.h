#ifndef NETLIST_INSTANCES_WIDGET_H
#define NETLIST_INSTANCES_WIDGET_H

#include <QMainWindow>
#include "Cell.h"
#include "InstancesModel.h"
#include <QTableView>
#include <QPushButton>
#include "CellViewer.h"


namespace Netlist {
	
	class InstancesWidget : public QWidget {
	    Q_OBJECT;
	  public:
		    ~InstancesWidget();
		    InstancesWidget ( QWidget* parent=NULL );
	    inline  void  setCellViewer   ( CellViewer* );
		    int   getSelectedRow  () const;
	    inline  void  setCell         ( Cell* );
	  public slots:
		    void  load            ();
	  private:
		    CellViewer*     cellViewer_;
		    InstancesModel* baseModel_;
		    QTableView*     view_;
		    QPushButton*    load_;
	};

	inline void InstancesWidget::setCell(Cell* c) {
		baseModel_->setCell(c);
	}

	inline void InstancesWidget::setCellViewer(CellViewer* c) {
		cellViewer_ = c;
	}
}

#endif
