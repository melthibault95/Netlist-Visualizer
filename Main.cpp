// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/7>" -*-

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "CellViewer.h"
using namespace Netlist;

#include  <QApplication>
#include  <QtGui>


int main ( int argc, char* argv[] )
{
  cout << "Chargement des modeles..." << endl;
  Cell::load( "vdd" );
  Cell::load( "gnd" );
  Cell::load( "TransistorN" );
  Cell::load( "TransistorP" );
  Cell::load( "and2" );
  Cell::load( "or2" );
  Cell* xor2      = Cell::load( "xor2" );
  Cell* halfadder = Cell::load( "halfadder" );

  cout << "\nContenu du <xor2>:" << endl;
  xor2->toXml( cout );

  cout << "\nContenu du <halfadder>:" << endl;
  halfadder->toXml( cout );

  QApplication* qa = new  QApplication(argc ,argv);
  CellViewer* viewer = new  CellViewer();
  viewer ->setCell( halfadder  );
  viewer ->show ();
  int  rvalue = qa ->exec();
  delete  qa;
  return  rvalue;
}
