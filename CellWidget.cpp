// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"


namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget::CellWidget ( QWidget* parent )
    : QWidget(parent)
    , cell_  (NULL)
    , viewport_ (Box(0,0,500,500))
  {
    setAttribute    ( Qt::WA_OpaquePaintEvent );
    setAttribute    ( Qt::WA_NoSystemBackground );
    setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy  ( Qt::StrongFocus );
    setMouseTracking( true );
  }


  CellWidget::~CellWidget ()
  { }


  void  CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize  CellWidget::minimumSizeHint () const
  { return QSize(500,500); }


  void   CellWidget :: resizeEvent(QResizeEvent* event) {
	const  QSize& size = event ->size ();
	//  Assume  the  resize  is  always  done by  drawing  the  bottom  right  corner.
	viewport_.setX2( viewport_.getX1() + size.width  () );
	viewport_.setY1( viewport_.getY2() - size.height () );
	cerr  << "CellWidget :: resizeEvent ()␣viewport_:" << viewport_  << endl;
  }

  void   CellWidget :: keyPressEvent(QKeyEvent* event) {
	event ->ignore ();
	if (event ->modifiers () & (Qt:: ControlModifier|Qt:: ShiftModifier ))
		return;
	switch( event ->key()) {
		case Qt:: Key_Up:    goUp    ();  break;
		case Qt:: Key_Down: goDown  ();  break;
		case Qt:: Key_Left: goLeft  ();  break;
		case Qt:: Key_Right:goRight ();  break;
		default: return;
	}
	event ->accept ();
  }

  void   CellWidget :: goRight () {
	viewport_.translate( Point (20,0) );
	repaint ();
  }
  void   CellWidget ::goUp() {
	viewport_.translate( Point (0 ,20) );
	repaint();
  }

  void   CellWidget ::goDown() {
	viewport_.translate( Point (0 ,-20) );
	repaint();
  }

  void   CellWidget ::goLeft() {
	viewport_.translate( Point (-20 ,0) );
	repaint();
  }



  void  CellWidget::paintEvent ( QPaintEvent* event )
  {
	if (not(cell_)) return;
	QPainter  painter(this);
	painter.setBackground( QBrush( Qt:: black ) );
	painter.eraseRect     ( QRect( QPoint (0,0), size() ) );

	query(painter);
  }

  void   CellWidget::query(QPainter& painter) {
	painter.setPen( QPen( Qt::darkGreen , 2 ) );
	const  vector <Instance*>& instances = cell_->getInstances ();
	vector<Instance*>::const_iterator iter_instance = instances.begin();

	// Instances Display
	for (; iter_instance != instances.end() ; ++iter_instance) {
		Point           instPos = (*iter_instance)->getPosition();
		const  Symbol* symbol   = (*iter_instance)->getMasterCell()->getSymbol ();
		if (not  symbol) continue;
		const  vector <Shape*>& shapes = symbol->getShapes ();
		vector<Shape*>::const_iterator iter_shape = shapes.begin();
		if (shapes.empty())
			continue;
		for (; iter_shape != shapes.end() ; ++iter_shape) {
			BoxShape* boxShape = dynamic_cast <BoxShape*>(*iter_shape);
			if (boxShape) {
				Box    box   = boxShape ->getBoundingBox ();
				QRect  rect = boxToScreenRect(box.translate(instPos ));
				painter.drawRect(rect);
			}
			
			EllipseShape* ellipseShape = dynamic_cast<EllipseShape*>(*iter_shape);
			if (ellipseShape) {
				Box    box   = ellipseShape ->getBoundingBox ();
				QRect  rect = boxToScreenRect(box.translate(instPos ));
				painter.drawEllipse(rect);
			}

			ArcShape* arcShape = dynamic_cast<ArcShape*>(*iter_shape);
			if (arcShape) {
				Box    box   = arcShape ->getBoundingBox();
				QRect  rect = boxToScreenRect(box.translate(instPos));
				painter.drawArc(rect, arcShape->getStart() * 16, arcShape->getSpan() * 16);
			}

			LineShape* lineShape = dynamic_cast<LineShape*>(*iter_shape);
			if (lineShape) {
				Point  pSource = lineShape->getSource();
				Point  pDestination = lineShape->getTarget();
				QPoint source = pointToScreenPoint(pSource.translate(instPos));
				QPoint destination = pointToScreenPoint(pDestination.translate(instPos));
				painter.drawLine(source, destination);
			}
		}
		
		QRect r = boxToScreenRect(symbol->getBoundingBox().translate(instPos));
		QRect label = QRect(r.center().x(), r.center().y() + 40, 50, 50);
		painter.drawText(label, Qt::AlignHCenter|Qt::AlignTop, (*iter_instance)->getName().c_str());
	}
	// Nets Display

	vector<Net*>::const_iterator iter_nets = cell_->getNets().begin();

	for (; iter_nets < cell_->getNets().end(); ++iter_nets) {
		painter.setPen( QPen( Qt::cyan , 0 ) );	
		painter.setBrush(QBrush(Qt::cyan));

		vector<Line*>::const_iterator iter_lines = (*iter_nets)->getLines().begin();

		for (; iter_lines < (*iter_nets)->getLines().end(); ++iter_lines) {
			QPoint source = pointToScreenPoint((*iter_lines)->getSourcePosition());
			QPoint destination = pointToScreenPoint((*iter_lines)->getTargetPosition());
			painter.drawLine(source, destination);
		}

		vector<Node*>::const_iterator iter_nodes = (*iter_nets)->getNodes().begin();

		for (; iter_nodes < (*iter_nets)->getNodes().end(); ++iter_nodes) {
			NodePoint* l = dynamic_cast<NodePoint*>(*iter_nodes);
			if (l == NULL) continue;
			QPoint p = pointToScreenPoint(l->getPosition());
			painter.drawEllipse(p, 5, 5);
		}
	}

	//Terms Display
	
	vector<Term*>::const_iterator iter_terms = cell_->getTerms().begin();

	for (; iter_terms < cell_->getTerms().end(); ++iter_terms) {
		painter.setPen( QPen( Qt::red , 0 ) );	
		painter.setBrush(QBrush(Qt::red));
		
		NodeTerm* n = (*iter_terms)->getNode();
		QPoint p = pointToScreenPoint(n->getPosition());
		painter.drawRect(p.x(), p.y() - 4, 10, 10);

		QRect label = QRect(p.x(), p.y(), 50, 50);
		painter.drawText(label, Qt::AlignHCenter|Qt::AlignTop, (*iter_terms)->getName().c_str());
	}

	//TermsInstance Display
	iter_instance = instances.begin();
	for (; iter_instance != instances.end() ; ++iter_instance) {
		painter.setPen( QPen( Qt::red , 0 ) );	
		painter.setBrush(QBrush(Qt::red));
		Point           instPos = (*iter_instance)->getPosition();
		const  Symbol* symbol   = (*iter_instance)->getMasterCell()->getSymbol ();
		if (not  symbol) continue;
		const  vector <Shape*>& shapes = symbol->getShapes ();
		vector<Shape*>::const_iterator iter_shape = shapes.begin();
		if (shapes.empty())
			continue;
		for (; iter_shape != shapes.end() ; ++iter_shape) {
			TermShape* termShape = dynamic_cast<TermShape*>(*iter_shape);
			if (termShape) {
				QPoint p = pointToScreenPoint(Point(termShape->getX(), termShape->getY()).translate(instPos));
				painter.drawRect(p.x(), p.y() - 2, 5, 5);

				QRect label = QRect(p.x() - 10, p.y() - 20, 20, 20);
				switch (termShape->getAlign()) {
					case TermShape::TopLeft: painter.drawText(label, Qt::AlignLeft|Qt::AlignTop, termShape->getTerm()->getName().c_str()); break;
					case TermShape::TopRight: painter.drawText(label, Qt::AlignRight|Qt::AlignTop, termShape->getTerm()->getName().c_str()); break;
					case TermShape::BottomLeft: painter.drawText(label, Qt::AlignLeft|Qt::AlignBottom, termShape->getTerm()->getName().c_str()); break;
					case TermShape::BottomRight: painter.drawText(label, Qt::AlignRight|Qt::AlignBottom, termShape->getTerm()->getName().c_str());
				}
				
			}
		}
	}	
	
   }


}  // Netlist namespace.
