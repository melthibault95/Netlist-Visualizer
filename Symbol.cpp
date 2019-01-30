#include "Symbol.h"
#include "Shape.h"
#include "Cell.h"
#include "Term.h"
#include "Indentation.h"


namespace Netlist {

                                        Symbol::Symbol          ( Cell* c):owner_(c), shapes_() { }
                                       	Symbol::~Symbol          () {}
             Cell*                      Symbol::getCell         () const {
						return owner_;
					}
             Box                        Symbol::getBoundingBox  () const {
						Box b = Box();
						std::vector<Shape*>::const_iterator it = shapes_.begin();
						for (; it != shapes_.end(); ++it)
							b = b.merge((*it)->getBoundingBox());
						return b;
					}
             Point                      Symbol::getTermPosition ( Term* t) const {
						TermShape* res = getTermShape(t);
						if (res)
							return Point(res->getX(), res->getY());
						return Point();
					}
             TermShape*                 Symbol::getTermShape    ( Term* t) const {
						TermShape* res = NULL;
						std::vector<Shape*>::const_iterator it = shapes_.begin();
						for (; it != shapes_.end(); ++it) {
							TermShape* sterm = dynamic_cast<TermShape *>(*it);
							if (sterm != NULL) {
								if (sterm->getTerm() == t) {
									res = sterm;
									break;
								}
							}
						}
						return res;
						
					}
             void                       Symbol::add             ( Shape* s) {
						shapes_.push_back(s);
					}
             void                       Symbol::remove          ( Shape* s) {
						std::vector<Shape*>::iterator it = shapes_.begin();
						for (; it != shapes_.end() && *it != s; ++it);
						if (*it == s)
							shapes_.erase(it);
					}
             void                       Symbol::toXml           ( std::ostream& o) const {
						    o << indent++ << "<symbol>\n";
						    for ( std::vector<Shape*>::const_iterator ishape=shapes_.begin();ishape != shapes_.end() ; ++ishape) {
						      (*ishape)->toXml( o );
						    }
						    o << --indent << "</symbol>\n";
					}
             Symbol*             	Symbol::fromXml         ( Cell* c, xmlTextReaderPtr reader) {
						  const xmlChar* symbolTag      = xmlTextReaderConstString( reader, (const xmlChar*)"symbol" );

						  Symbol* res   = c->getSymbol();				    

						  while ( true ) {
						    int status = xmlTextReaderRead(reader);
						    if (status != 1) {
						      if (status != 0) {
							std::cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << std::endl;
						      }
						      break;
						    }

						    switch ( xmlTextReaderNodeType(reader) ) {
						      case XML_READER_TYPE_COMMENT:
						      case XML_READER_TYPE_WHITESPACE:
						      case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
							continue;
						    }

						    const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

						    if ( (nodeName == symbolTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ) {
							  break;
						    }
						    else
					  		if (Shape::fromXml(res,reader)) continue;
				
						    std::cerr << "[ERROR] Symbol::fromXml(): Unknown or misplaced tag <" << nodeName
							 << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
						    break;
						  }

						  return res;
					}

                                	Symbol::Symbol          ( const Symbol& sym) {	}
             Symbol&                    Symbol::operator=       ( const Symbol& sym) {  }
}
