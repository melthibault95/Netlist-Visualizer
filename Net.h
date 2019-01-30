#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <vector>
#include <string>
#include <iostream>

#include "Term.h"
#include "Line.h"

namespace Netlist {

	class Node;
	class Cell;

	class Net {
		private:
			Cell*		owner_;
			std::string	name_;
			unsigned int	id_;
			Term::Type	type_;
			std::vector<Node*>	nodes_;
			std::vector<Line*> 	lines_;
		public:
			// Constructeurs/Destructeurs
					Net(Cell*, const std::string&, Term::Type);
					~Net();			
			static 		Net*	  	fromXml(Cell*, xmlTextReaderPtr);
			// Accesseurs
					Cell*		getCell() const;
			const 		std::string&	getName() const;
			unsigned 	int		getId() const;
					Term::Type	getType() const;
			const 		std::vector<Node*>&	getNodes() const;
					size_t		getFreeNodeId() const;
		inline 	const 		std::vector<Line*>& getLines () const;
	 				Node* 		getNode(int id) const;
			// Mutateurs
					void		add(Node*);
					bool		remove(Node*);
        				bool  		remove ( Line* line );
					void  		add ( Line* line );
					void		toXml (std::ostream&) const;
	};

	inline const std::vector<Line*>& Net::getLines () const	{ return lines_; }
}

#endif
