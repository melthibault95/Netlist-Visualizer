#include "Net.h"
#include "Node.h"
#include "XmlUtil.h"

namespace Netlist {
	Net::Net(Cell* c, const std::string& name, Term::Type t) :name_(name), owner_(c), id_(c->newNetId()), type_(t), nodes_(), lines_(){
		c->add(this);
	}
	Net::~Net() {
		std::vector<Node*>::const_iterator it = nodes_.begin();
		for (; it != nodes_.end(); ++it) {
			NodeTerm* n = dynamic_cast<NodeTerm *>(*it);
			if (n != NULL)
				n->getTerm()->setNet(NULL);
		}

		getCell()->remove(this);
	}
		
			Cell* 			Net::getCell() const {
				return owner_;
			}
	const 		std::string& 		Net::getName() const {
				return name_;
			}
	unsigned 	int 			Net::getId() const {
				return id_;
			}
			Term::Type 		Net::getType() const {
				return type_;
			}
	const 		std::vector<Node*>& 	Net::getNodes() const {
				return nodes_;
			}

 	 		Node* 	 Net::getNode(int id) const {
				if (id >= nodes_.size() && id < 0) {
					std::cerr << "Erreur 404: Id erroné" << std::endl;
					exit(1);
				}
				return nodes_[id];
			}

			void  Net::add ( Line* line ){ 
				if (line) lines_.push_back( line );
			}

			bool  Net::remove ( Line* line )
			{
			  if (line) {
			    for ( std::vector<Line*>::iterator il = lines_.begin(); il != lines_.end() ; ++il ) {
			      if (*il == line) {
				lines_.erase( il );
				return true;
			      }
			    }
			  }
			  return false;
			}

			size_t 			Net::getFreeNodeId() const {
				std::vector<Node*>::const_iterator it = nodes_.begin();
				size_t id = 0;
				for (; it != nodes_.end() && *it != NULL; ++it)
					++id;
				return id;
			}

			void 			Net::add(Node* n) {
				if (n != NULL) {
					size_t id = 0;
					if (n->getId() != Node::noid) {
						if (n->getId() >= nodes_.size()) {
							while(n->getId() >= nodes_.size())
								nodes_.push_back(NULL);
							nodes_[n->getId()] = n;
						}
						else {
							if (nodes_[n->getId()] == NULL)
								nodes_[n->getId()] = n;
						}
					}
					else {
						id = getFreeNodeId();
						if (id == nodes_.size())
							nodes_.push_back(n);
						else 
							nodes_[id] = n;
						n->setId(id);
					}
				}
			}

			bool 			Net::remove(Node* n) {
				if (n != NULL) {
					if ((n->getId() >= nodes_.size()) || (n->getId() == Node::noid))
						return true;
					nodes_[n->getId()] = NULL;
					if (nodes_[n->getId()] != NULL)
						return false;
					n->setId(Node::noid);
					return true;
				}
				return true;
			}
			void			Net::toXml (std::ostream& o) const{
				o << indent++ << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) << "\"/>\n";
				std::vector<Node*>::const_iterator it_nodes = nodes_.begin();
				for (; it_nodes < nodes_.end(); ++it_nodes)
				{
					if(*it_nodes !=NULL)
						(*it_nodes)->toXml(o);
				}
				std::vector<Line*>::const_iterator it_lines = lines_.begin();
				for (; it_lines < lines_.end(); ++it_lines)
				{
					if(*it_lines !=NULL)
						(*it_lines)->toXml(o);
				}
				o << --indent << "</net>\n";
			}

			Net* Net::fromXml (Cell* cel, xmlTextReaderPtr reader )
			{
			  const xmlChar* netTag      = xmlTextReaderConstString( reader, (const xmlChar*)"net" );

			  Net* res   = NULL;

			  bool line = false;

			  std::string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
			  std::string netType = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type"));
			  if (not netName.empty() and not netType.empty()) {
			    Term::Type t = "Internal" == netType?Term::Internal:Term::External;
			    res = new Net ( cel, netName, t );
			  }
			  else
				return res;
				    

			  while ( true ) {
			    int status = xmlTextReaderRead(reader);
			    if (status != 1) {
			      if (status != 0) {
				std::cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << std::endl;
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
				

			    if ( (nodeName == netTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ) {
				  break;
			    }
			    else {
		  		if (!line && Node::fromXml(res,reader)) continue;
				else if (Line::fromXml(res, reader))
					line = true;
				continue;

			    }
				
			    std::cerr << "[ERROR] Net::fromXml(): Unknown or misplaced tag <" << nodeName
				 << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
			    break;
			  }

			  return res;
			}
}
