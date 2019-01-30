#include "CellViewer.h"
#include "OpenCellDialog.h"
#include "InstancesWidget.h"

namespace Netlist {

	CellViewer :: CellViewer ( QWidget* parent ): 
	  QMainWindow(parent)
	, cellWidget_     (NULL)
	, saveCellDialog_ (NULL)
	, instancesWidget_(NULL)
	, cellsLib_	  (NULL)	
	{
		cellWidget_      = new  CellWidget  ();
		saveCellDialog_ = new  SaveCellDialog( this );
		instancesWidget_ = new InstancesWidget();
		cellsLib_	 = new CellsLib();
		instancesWidget_->setCellViewer(this);
		cellsLib_->setCellViewer(this);
		setCentralWidget( cellWidget_  );
		QMenu* fileMenu = menuBar()->addMenu( "File" );

		QAction* action = new  QAction( "Open Cell", this );
		action ->setStatusTip( "Open a new Cell" );
		action ->setShortcut ( QKeySequence("CTRL+O") );
		action ->setVisible   ( true );
		fileMenu ->addAction( action  );
		connect( action , SIGNAL(triggered ()), this , SLOT(openCell()));

		action = new  QAction( "Save As", this );
		action ->setStatusTip( "Save to disk (rename) the Cell" );
		action ->setShortcut ( QKeySequence("CTRL+S") );
		action ->setVisible   ( true );
		fileMenu ->addAction( action  );
		connect( action , SIGNAL(triggered ()), this , SLOT(saveCell()));

		action = new  QAction( "Quit", this );
		action ->setStatusTip( "Exit the Netlist Viewer" );
		action ->setShortcut ( QKeySequence("CTRL+Q") );
		action ->setVisible   ( true );
		fileMenu ->addAction( action  );
		connect( action , SIGNAL(triggered ()), this , SLOT(close ()));

		connect(this, SIGNAL(cellLoaded()), cellsLib_->getBaseModel(), SLOT(updateDatas()));

		showCellsLib();
		showInstancesWidget();

	}

	CellViewer::~CellViewer() { 
		delete cellWidget_;
		delete saveCellDialog_;
		delete instancesWidget_;
		delete cellsLib_;
	}

	void CellViewer::openCell() {
		Cell* c = getCell();
		QString  cellName;
		if (c)
			cellName = c->getName().c_str();
		if (OpenCellDialog::run(cellName)) {
			if ((c = Cell::find(cellName.toStdString())) == NULL) {
				c = Cell::load(cellName.toStdString());
				emit cellLoaded();
			}
			setCell(c);
		}
		
	}

	void CellViewer::saveCell() {
		Cell* cell = getCell();
		if (cell == NULL) return;
		QString  cellName = cell->getName(). c_str();
		if (saveCellDialog_->run(cellName)) {
			cell->setName(cellName.toStdString());
			cell->save(cellName.toStdString());
		}
	}

	Cell* CellViewer::getCell() const {
		return cellWidget_->getCell();
	}

	void CellViewer::setCell(Cell* c) {
		cellWidget_->setCell(c);
		instancesWidget_->setCell(c);
	}

	void CellViewer::showInstancesWidget() {
		instancesWidget_->show();
	}

	void CellViewer::showCellsLib() {
		cellsLib_->show();
	}
}
