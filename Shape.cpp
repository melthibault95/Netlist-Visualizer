#include "Shape.h"
#include "Cell.h"
#include "XmlUtil.h"
#include "Term.h"

namespace Netlist {

	Shape::Shape(Symbol* s) :owner_(s) { 
		s->add(this);
	}
	Shape::~Shape() {
		owner_->remove(this);
	}

	Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
	{
		// Factory-like method.
		const xmlChar* boxTag
		= xmlTextReaderConstString( reader, (const xmlChar*)"box" );
		const xmlChar* ellipseTag
		= xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
		const xmlChar* arcTag
		= xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
		const xmlChar* lineTag
		= xmlTextReaderConstString( reader, (const xmlChar*)"line" );
		const xmlChar* termTag
		= xmlTextReaderConstString( reader, (const xmlChar*)"term" );
		const xmlChar* nodeName
		  = xmlTextReaderConstLocalName( reader );

		Shape* shape = NULL;
		if (boxTag == nodeName)
			shape = BoxShape::fromXml( owner, reader );
		if (ellipseTag == nodeName)
			shape = EllipseShape::fromXml( owner, reader );
		if (arcTag == nodeName)
			shape = ArcShape::fromXml( owner, reader );
		if (lineTag == nodeName)
			shape = LineShape::fromXml( owner, reader );
		if (termTag == nodeName)
			shape = TermShape::fromXml( owner, reader );

		if (shape == NULL)
			std::cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
		    	 << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

		return shape;
	}




	BoxShape::BoxShape(Symbol* s, const Box& b) :Shape(s), box_(b) { }
	BoxShape::BoxShape(Symbol* s, int x1, int y1, int x2, int y2) :Shape(s), box_(x1, x2, y1, y2) {}
	BoxShape::~BoxShape() {	}

	Shape* BoxShape::fromXml(Symbol* s, xmlTextReaderPtr reader) {
		BoxShape* res = NULL;
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
		if (xmlGetIntAttribute( reader, "x1", x1 ) and  xmlGetIntAttribute( reader, "y1", y1) and xmlGetIntAttribute( reader, "x2", x2 ) and  				xmlGetIntAttribute( reader, "y2", y2))
			res = new BoxShape(s, x1, y1, x2, y2);
		return res;
	}

	void	BoxShape::toXml(std::ostream& o) const {
		o << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() <<  "\" y2=\"" << box_.getY2() 			<< "\" />\n";
	}

	LineShape::LineShape(Symbol* s, int x1, int y1, int x2, int y2):Shape(s), x1_(x1), x2_(x2), y1_(y1), y2_(y2) {}

	LineShape::~LineShape() {}

	Shape* LineShape::fromXml(Symbol* s, xmlTextReaderPtr reader) {
		LineShape* res = NULL;
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
		if (xmlGetIntAttribute( reader, "x1", x1 ) and  xmlGetIntAttribute( reader, "y1", y1) and xmlGetIntAttribute( reader, "x2", x2 ) and  				xmlGetIntAttribute( reader, "y2", y2))
			res = new LineShape(s, x1, y1, x2, y2);
		return res;
	}

	void	LineShape::toXml(std::ostream& o) const {
		o << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" x2=\"" << x2_ <<  "\" y2=\"" << y2_ << "\" />\n";
	}

	TermShape::TermShape(Symbol* s, std::string name, int x1, int y1, NameAlign align):Shape(s), term_(NULL) ,x1_(x1), y1_(y1), align_(align) {
		term_ = s->getCell()->getTerm(name);
	}
	TermShape::~TermShape() {}

	Shape* TermShape::fromXml(Symbol* s, xmlTextReaderPtr reader) {
		TermShape* res = NULL;
		std::string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
		std::string termAlign = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ) );
		int x = 0;
		int y = 0;
		if (not termName.empty() and not termAlign.empty() and xmlGetIntAttribute( reader, "x1", x ) and  xmlGetIntAttribute( reader, "y1", y ))
			res = new TermShape(s, termName, x, y, TermShape::toAlign(termAlign));
		return res;
	}

	void	TermShape::toXml(std::ostream& o) const {
		o << indent << "<term name=\"" << getTerm()->getName() << "\" x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" align=\"" << toString(align_) << 			"\" />\n";
	}

	TermShape::NameAlign	TermShape::toAlign(std::string s) {
		if (s == "top_left")		return TopLeft;
		if (s == "top_right")		return TopRight;
		if (s == "bottom_left")  	return BottomLeft;
		if (s == "bottom_right") 	return BottomRight;
		else				return Unknown;
	}

	std::string 		TermShape::toString(TermShape::NameAlign a) {
		switch(a) {
			case TopLeft: 	 return "top_left";
			case TopRight: 	 return "top_right";
			case BottomLeft: return "bottom_left";
			case BottomRight:return "bottom_right";
			case Unknown: 	 return "unknown";
			default:	 return "pire qu'unknown";
		}
	}

	EllipseShape::EllipseShape(Symbol* s, int x1, int y1, int x2, int y2):Shape(s), box_(x1, y1, x2, y2) {}
	EllipseShape::~EllipseShape() {	}

	void 	EllipseShape::toXml(std::ostream& o) const {
		o << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() <<  "\" y2=\"" 
		<< box_.getY2() << "\" />\n";
	}

	Shape* EllipseShape::fromXml(Symbol* s, xmlTextReaderPtr reader) {
		EllipseShape* res = NULL;
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
		if (xmlGetIntAttribute( reader, "x1", x1 ) and  xmlGetIntAttribute( reader, "y1", y1) and xmlGetIntAttribute( reader, "x2", x2 ) and  				xmlGetIntAttribute( reader, "y2", y2))
			res = new EllipseShape(s, x1, y1, x2, y2);
		return res;
	}

	ArcShape::ArcShape(Symbol* owner, int x1, int y1, int x2, int y2, int start, int span)
		: Shape(owner),
		box_ (x1, y1, x2, y2),
		start_(start),
		span_(span)
	{}

	ArcShape::~ArcShape() {}

	Box	ArcShape::getBoundingBox()const
	{
		return box_;
	}

	int	ArcShape::getStart()const
	{
		return start_;
	}

	int	ArcShape::getSpan()const
	{
		return span_;
	}

	Shape* ArcShape::fromXml(Symbol* s, xmlTextReaderPtr reader){
		ArcShape* res = NULL;
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
		int start = 0;
		int span = 0;
		if (xmlGetIntAttribute( reader, "x1", x1 ) and  xmlGetIntAttribute( reader, "y1", y1) and xmlGetIntAttribute( reader, "x2", x2 ) and  				xmlGetIntAttribute( reader, "y2", y2) and xmlGetIntAttribute( reader, "start", start ) and  							xmlGetIntAttribute( reader, "span", span))
			res = new ArcShape(s, x1, y1, x2, y2, start, span);

		return res;
	}

	void ArcShape::toXml(std::ostream& o) const{
		o << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() <<  "\" y2=\"" 
		<< box_.getY2() << "\" start=\"" << start_ << "\" span=\"" << span_ << "\" />\n";
	}
}


