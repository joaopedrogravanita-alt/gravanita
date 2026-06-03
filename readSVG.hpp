#ifndef __svg_readSVG_hpp__
#define __svg_readSVG_hpp__

#include <string>
#include <vector>
#include "SVGElements.hpp"
#include "Point.hpp"

namespace svg {
    void readSVG(const std::string &svg_file, Point &dimensions, std::vector<SVGElement *> &svg_elements);
}

#endif