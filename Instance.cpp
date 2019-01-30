#include  "Instance.h"
#include "Term.h"
#include "Net.h"
#include "XmlUtil.h"
#include "Shape.h"
#include <cstdlib>

namespace Netlist {
		Instance::Instance 	 (Cell* owner, Cell* model, const std::string& name) :owner_(owner), masterCell_(model), name_(name), terms_(){
			std::vector<Term*> vec = masterCell_->getTerms();
			for (std::vector<Term*>::const_iterator it = vec.begin(); it != vec.end(); ++it)
				new Term(this, *it);

			owner_->add(this);
		}
	       	Instance::~Instance      () {
			std::vector<Term*>::const_iterator it;
			for(it = terms_.begin(); it != terms_.end(); ++it) {
				delete *it;
			}
		}

		const std::string&        Instance::getName       () const {
			return name_;
		}
		Cell*                     Instance::getMasterCell () const {
			return masterCell_;
		}
		Cell*                     Instance::getCell       () const {
			return owner_;
		}
		const std::vector<Term*>& Instance::getTerms      () const {
			return terms_;
		}
		Term*                     Instance::getTerm       (const std::string& str) const {
			std::vector<Term*>::const_iterator it;
			for(it = terms_.begin(); it != terms_.end() && str != (*it)->getName(); ++it);
			if (it != terms_.end())
				return *it;
			return NULL;
		}
		Point                     Instance::getPosition   () const {
			return position_;
		}

		bool  			  Instance::connect      (const std::string& name, Net* n) {
			Term* t = getTerm(name);
			if (t == NULL)
				return false;
			n->add(t->getNode());
			return true;
		}
		void  			  Instance::add           (Term* t){
			terms_.push_back(t);
		}
		void  			  Instance::remove        (Term* t){
			std::vector<Term*>::iterator it;
			for(it = terms_.begin(); it != terms_.end() && t != *it; ++it);
			if (it != terms_.end())
				terms_.erase(it);
					
		}
		void Instance::setPosition( const  Point& p){
			position_ = p;
			std::vector<Shape*> s = getMasterCell()->getSymbol()->getShapes();
		
			for (std::vector<Shape*>::const_iterator it = s.begin() ; it < s.end() ; it++) {
				TermShape* ts = dynamic_cast<TermShape*>(*it);
				if (ts != NULL) {
					Term* ti = getTerm(ts->getTerm()->getName());
					ti->setPosition(getPosition().translate(ts->getX(), ts->getY()));
				}

			}
		}
		void  			  Instance::setPosition   (int x, int y){
			setPosition(Point(x, y));
		}
		void			  Instance::toXml (std::ostream& o) {
			o << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName() << "\" x=\"" << getPosition().getX() << "\" y=\"" << getPosition().getY() << "\"/>\n";
		}

	Instance* Instance::fromXml(Cell* cel, xmlTextReaderPtr reader){

	  Instance* res = NULL;
	  std::string InstanceName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
	  std::string InstanceMastercell = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ) );
	  int x = 0;
	  int y = 0;

	  if (not InstanceName.empty() and not InstanceMastercell.empty() and xmlGetIntAttribute( reader, "x", x ) and  xmlGetIntAttribute( reader, "y", y )) {
	    Cell* masterc = Cell::find(InstanceMastercell);
	  	if(masterc!=NULL){
		    	res = new Instance(cel, masterc, InstanceName );
			res->setPosition(x, y);
		}
	  }
	  
	return res;
     }
}


