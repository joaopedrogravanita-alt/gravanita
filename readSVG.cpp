#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    // Helper to map string names and hex codes to Color objects
    Color stringToColor(const std::string& colorStr) {
        if (colorStr.empty()) return Color{0, 0, 0};

        // Hexadecimal color parsing (#RRGGBB)
        if (colorStr[0] == '#') {
            if (colorStr.length() == 7) {
                int r, g, b;
                std::stringstream ss;
                ss << std::hex << colorStr.substr(1, 2); ss >> r;
                ss.clear();
                ss << std::hex << colorStr.substr(3, 2); ss >> g;
                ss.clear();
                ss << std::hex << colorStr.substr(5, 2); ss >> b;
                return Color{(unsigned char)r, (unsigned char)g, (unsigned char)b};
            }
        }

        // Named color mappings
        if (colorStr == "red")    return Color{255, 0, 0};
        if (colorStr == "blue")   return Color{0, 0, 255};
        if (colorStr == "green")  return Color{0, 255, 0};
        if (colorStr == "white")  return Color{255, 255, 255};
        if (colorStr == "black")  return Color{0, 0, 0};
        if (colorStr == "yellow") return Color{255, 255, 0};
        
        return Color{0, 0, 0}; // Default
    }

    std::vector<Point> parsePoints(const std::string& pointsStr) {
        std::vector<Point> points;
        std::string cleanStr = pointsStr;
        for (char& c : cleanStr) if (c == ',') c = ' ';
        std::stringstream ss(cleanStr);
        int x, y;
        while (ss >> x >> y) points.push_back({x, y});
        return points;
    }
    
    Point parseOrigin(const char* attrStr) {
        if (!attrStr) return {0, 0};
        std::stringstream ss(attrStr);
        double x = 0, y = 0;
        ss >> x >> y;
        return {static_cast<int>(x), static_cast<int>(y)};
    }
    
    SVGElement* readElement(XMLElement* elem, std::map<std::string, SVGElement*>& idMap) {
        std::string name = elem->Value();
        SVGElement* svgElem = nullptr;
        std::string transform = elem->Attribute("transform") ? elem->Attribute("transform") : "";
        Point transformOrigin = parseOrigin(elem->Attribute("transform-origin"));
    
        if (name == "ellipse") {
            Color fill = stringToColor(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            svgElem = new Ellipse(fill, {elem->IntAttribute("cx"), elem->IntAttribute("cy")}, 
                                        {elem->IntAttribute("rx"), elem->IntAttribute("ry")}, transform, transformOrigin);
        }
        else if (name == "circle") {
            Color fill = stringToColor(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            svgElem = new Circle(fill, {elem->IntAttribute("cx"), elem->IntAttribute("cy")}, 
                                       elem->IntAttribute("r"), transform, transformOrigin);
        }
        else if (name == "polyline") {
            Color stroke = stringToColor(elem->Attribute("stroke") ? elem->Attribute("stroke") : "");
            svgElem = new Polyline(stroke, parsePoints(elem->Attribute("points") ? elem->Attribute("points") : ""), transform, transformOrigin);
        }
        else if (name == "line") {
            Color stroke = stringToColor(elem->Attribute("stroke") ? elem->Attribute("stroke") : "");
            svgElem = new Line(stroke, {elem->IntAttribute("x1"), elem->IntAttribute("y1")}, 
                                       {elem->IntAttribute("x2"), elem->IntAttribute("y2")}, transform, transformOrigin);
        }
        else if (name == "polygon") {
            Color fill = stringToColor(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            svgElem = new Polygon(fill, parsePoints(elem->Attribute("points") ? elem->Attribute("points") : ""), transform, transformOrigin);
        }
        else if (name == "rect") {
            Color fill = stringToColor(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            svgElem = new Rect(fill, {elem->IntAttribute("x"), elem->IntAttribute("y")}, 
                                     elem->IntAttribute("width"), elem->IntAttribute("height"), transform, transformOrigin);
        }
        else if (name == "g") {
            Group* group = new Group(transform, transformOrigin);
            for (XMLElement* child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
                SVGElement* childElem = readElement(child, idMap); 
                if (childElem) group->addElement(childElem);
            }
            svgElem = group;
        }
        else if (name == "use") {
            const char* href = elem->Attribute("href");
            if (href && href[0] == '#' && idMap.count(href + 1)) {
                svgElem = new Use(idMap[href + 1]->clone(), transform, transformOrigin);
            }
        }
    
        if (svgElem && elem->Attribute("id")) {
            idMap[elem->Attribute("id")] = svgElem;
        }
        return svgElem;
    }
    
    void readSVG(const std::string &svg_file, Point &dimensions, std::vector<SVGElement *> &svg_elements) {
        XMLDocument doc;
        if (doc.LoadFile(svg_file.c_str()) != XML_SUCCESS) return;
        XMLElement* svgRoot = doc.FirstChildElement("svg");
        if (!svgRoot) return;
    
        dimensions = {svgRoot->IntAttribute("width"), svgRoot->IntAttribute("height")};
        std::map<std::string, SVGElement*> idMap;
    
        for (XMLElement* elem = svgRoot->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
            SVGElement* svgElem = readElement(elem, idMap);
            if (svgElem) svg_elements.push_back(svgElem);
        }
    }
}