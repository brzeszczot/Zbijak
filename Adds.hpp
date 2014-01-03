/*
 * Vertex.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: agolebiewski
 */

#ifndef VERTEX_HPP_
#define VERTEX_HPP_

class Adds
{
	public:
		typedef struct vertex
		{
			float x, y;
		} vertex;
		Adds();
		virtual ~Adds();
		Adds& operator=(const Adds &a)
		{
		//	vertex.x = a.x;
		//	vertex.y = a.y;
		//	return *this;
		}
};


#endif /* VERTEX_HPP_ */
