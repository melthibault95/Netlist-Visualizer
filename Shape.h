#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include "Symbol.h"
#include <iostream>

namespace Netlist {
	class Shape {
		public:
		//Constructeur
		Shape		(Symbol*);
		//Destructeur
	virtual	~Shape		();

	inline	Symbol*	getSymbol () const;
		//Accesseur
	virtual Box	getBoundingBox	() const = 0;

	static 	Shape*	fromXml(Symbol*, xmlTextReaderPtr);
	virtual	void	toXml(std::ostream&) const = 0;

		private:
		Symbol* owner_;
		
		
	};

	class BoxShape : public Shape {
		public:
		//Constructeur
		BoxShape	(Symbol*, const Box&);
		BoxShape	(Symbol*, int x1, int y1, int x2, int y2);
		//Destructeur
		~BoxShape	();

	inline	Box	getBoundingBox() const;

	static 	Shape*	fromXml(Symbol*, xmlTextReaderPtr);
		void	toXml(std::ostream&) const;

		private:
		Box	box_;
	};

	class LineShape : public Shape {
		public:
		//Constructeur
		LineShape	(Symbol*, int x1, int y1, int x2, int y2);
		//Destructeur
		~LineShape	();

	inline	Box	getBoundingBox() const;
	inline  Point	getSource()	 const;
	inline  Point	getTarget()	 const;

	static 	Shape*	fromXml(Symbol*, xmlTextReaderPtr);
		void	toXml(std::ostream&) const;

		private:
		int	x1_, x2_, y1_, y2_;
	};

	class EllipseShape : public Shape {
		public:
			//Constructeur-Destructeur
			EllipseShape(Symbol*, int x1, int y1, int x2, int y2);
			~EllipseShape();
			//Accesseur
		inline 	Box 	getBoundingBox() const;
			void 	toXml(std::ostream&) const;
			static 	Shape* fromXml(Symbol* s, xmlTextReaderPtr reader);
		private:
			Box	box_;
	};

	class ArcShape : public Shape {
		public:
			//Constructeur-Destructeur
			ArcShape(Symbol*, int x1, int y1, int x2, int y2, int start, int span);
			~ArcShape();
			//Accesseur
			Box	getBoundingBox() const;
			int getStart() const;
			int getSpan() const;
			void toXml(std::ostream&) const;
			static Shape* fromXml(Symbol* s, xmlTextReaderPtr reader);

		private:
			Box    box_;
			int		start_;
			int		span_;
	};

	class TermShape : public Shape {
		public:
		enum NameAlign { TopLeft = 1, TopRight, BottomLeft, BottomRight, Unknown };
		//Constructeur
		TermShape	(Symbol*, std::string name, int x1, int y1, NameAlign align);
		//Destructeur
		~TermShape	();

	inline	Box	getBoundingBox() const;
	inline  Term*	getTerm	      () const;
	inline	int	getX	      () const;
	inline	int	getY	      () const;
	inline  NameAlign getAlign    () const;

	static 	Shape*	fromXml(Symbol*, xmlTextReaderPtr);
	static  NameAlign toAlign(std::string s);
	static  std::string toString(NameAlign a);
		void	toXml(std::ostream&) const;

		private:
		Term*	term_;
		int	x1_, y1_;
		NameAlign align_;
	};

	inline  Term*	TermShape::getTerm    () const {
		return term_;
	}
	inline	int	TermShape::getX	      () const {
		return x1_;
	}
	inline	int	TermShape::getY	      () const {
		return y1_;
	}

	inline  TermShape::NameAlign TermShape::getAlign () const {
		return align_;
	}

	inline 	Box	BoxShape::getBoundingBox() const {
		return box_;
	}

	inline 	Box	TermShape::getBoundingBox() const {
		return Box(x1_, y1_, x1_, y1_);
	}

	inline 	Box	LineShape::getBoundingBox() const {
		return Box(x1_, y1_, x2_, y2_);
	}

	inline  Point	LineShape::getSource() const {
		return Point(x1_, y1_);
	}

	inline  Point	LineShape::getTarget() const {
		return Point(x2_, y2_);
	}

	inline	Box	EllipseShape::getBoundingBox() const {
		return	box_;
	}
	
	
}

#endif
