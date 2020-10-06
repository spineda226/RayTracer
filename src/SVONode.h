/**
 * SVONode.h
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef _SVO_NODE_H_
#define _SVO_NODE_H_

#include <iostream>
#include <stdint.h>

class SVONode
{
   public:
   		SVONode();
   		~SVONode();
   		void* childPointers[8];

   		bool operator< ( const SVONode & val ) const;
   		bool operator!= ( const SVONode & other ) const;
   		bool operator== ( const SVONode & other ) const;
   		void print();
   		void printOneLine();
    private:
    	

};

#endif