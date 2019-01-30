// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


class CellWidget : public QWidget {
    Q_OBJECT;
  public:
                    CellWidget         ( QWidget* parent=NULL );
    virtual        ~CellWidget         ();
            void    setCell            ( Cell* );
    inline  Cell*   getCell            () const;
    inline  int     xToScreenX         ( int x ) const;
    inline  int     yToScreenY         ( int y ) const;
    inline  QRect   boxToScreenRect    ( const Box& ) const;
    inline  QPoint  pointToScreenPoint ( const Point& ) const;
    inline  int     screenXToX         ( int x ) const;
    inline  int     screenYToY         ( int y ) const;
    inline  Box     screenRectToBox    ( const QRect& ) const;
    inline  Point   screenPointToPoint ( const QPoint& ) const;
    virtual QSize   minimumSizeHint    () const;
    virtual void    resizeEvent        ( QResizeEvent* );
  protected:
    virtual void    paintEvent         ( QPaintEvent* );
    virtual void    keyPressEvent      ( QKeyEvent* );
  public slots:
            void    goLeft             ();
            void    goRight            ();
            void    goUp               ();
            void    goDown             ();
  private:
    Cell* cell_;
    Box   viewport_;
    	    void    query	       (QPainter& painter);
};




  inline Cell* CellWidget::getCell () const { return cell_; }

  inline int   CellWidget::xToScreenX	      (int x) const { return x - viewport_.getX1(); }
  inline int   CellWidget::yToScreenY	      (int y) const { return viewport_.getY2() - y; }
  inline int   CellWidget::screenXToX         (int x) const { return x + viewport_.getX1(); }
  inline int   CellWidget::screenYToY         (int y) const { return viewport_.getY2() - y; }

  inline QRect   CellWidget::boxToScreenRect    ( const Box& b ) const {
	return QRect(xToScreenX(b.getX1()), yToScreenY(b.getY1()), b.getX2() - b.getX1(), b.getY1() - b.getY2());
  }

  inline QPoint  CellWidget::pointToScreenPoint ( const Point& p ) const { 
	return QPoint(xToScreenX(p.getX()), yToScreenY(p.getY()));
  }

  inline Box     CellWidget::screenRectToBox    ( const QRect& qr) const  { 
	return Box(screenXToX(qr.x()), screenYToY(qr.y()), screenXToX(qr.x() + qr.width()), screenYToY(qr.y() - qr.height())); 
  }
  inline Point   CellWidget::screenPointToPoint ( const QPoint& qp ) const   { 
	return Point(screenXToX(qp.x()), screenYToY(qp.y()));
  }



}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H
