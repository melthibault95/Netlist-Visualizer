#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "XmlUtil.h"

namespace Netlist {

	Term::Term(Cell* c, const std::string& name, Direction d) :node_(this), type_(External), name_(name), direction_(d), owner_(c), net_(NULL){
		c->add(this);
	}
		
	Term::Term(Instance* i, const Term* modelTerm) :node_(this), type_(Internal), name_(modelTerm->getName()), direction_(modelTerm->getDirection()), owner_(i), net_(modelTerm->getNet()){
		i->add(this);
	}
	Term::~Term() {
		if (isInternal())
			getInstance()->remove(this);
		else
			getCell()->remove(this);
	}

	std::string	Term::toString(Term::Type t) {
		switch(t) {
			case Internal: return "Internal";
			case External: return "External";
			default: return "/";
		}
	}

	std::string	Term::toString(Term::Direction d) {
		switch(d) {
			case In: return "In";
			case Out: return "Out";
			case Inout: return "Inout";
			case Tristate: return "Tristate";
			case Transcv: return "Transcv";
			case Unknown: return "Unknown";
			default: return "Pire qu'unknown";
		}
	}

	Term::Direction	Term::toDirection(std::string s) {
		if (s == "In")		return In;
		if (s == "Out")		return Out;
		if (s == "Inout")  	return Inout;
		if (s == "Tristate") 	return Tristate;
		if (s == "Transcv") 	return Transcv;
		return Unknown;
	}

	void		Term::setNet(Net* n) {
		if (n == NULL) {
			net_->remove(&node_);
			net_ = NULL;
		}
		else if (net_ == NULL) {
			n->add(&node_);
			net_ = n;
		}
		else {
			if (net_->remove(&node_)) {
				n->add(&node_);
				net_ = n;
			}
		}
	}

	Cell*		Term::getOwnerCell() const {
		if (isInternal())
			return getInstance()->getCell();
		return getCell();
	}

	void		Term::setNet(const std::string& s) {
		setNet(getOwnerCell()->getNet(s));
	}
	void		Term::setPosition(const Point& p) {
		 node_.setPosition(p);
	}
	void		Term::setPosition(int x, int y) {
		node_.setPosition(x,y);
	}

	void		Term::toXml (std::ostream& o) const {
		o << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_) << "\"/>\n";
	}


	Term* Term::fromXml(Cell* cel, xmlTextReaderPtr reader) {
		Term* res = NULL;
		std::string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
		std::string termDirection = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );
		int x = 0;
		int y = 0;
		if (not termName.empty() and not termDirection.empty() and xmlGetIntAttribute( reader, "x", x ) and  xmlGetIntAttribute( reader, "y", y )) {
			res = new Term(cel, termName, Term::toDirection(termDirection));
			res->setPosition(x, y);
			return res;
		}
		return res;
     }


}
