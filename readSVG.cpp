
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        // TODO complete code -->
        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            string name = child->Value();

            if (name == "ellipse")
            {
                Color fill = parse_color(child->Attribute("fill"));
                Point center = { child->IntAttribute("cx"), child->IntAttribute("cy") };
                Point radius = { child->IntAttribute("rx"), child->IntAttribute("ry") };
                svg_elements.push_back(new Ellipse(fill, center, radius));
            }
            else if (name == "circle")
            {
                Color fill = parse_color(child->Attribute("fill"));
                Point center = { child->IntAttribute("cx"), child->IntAttribute("cy") };
                int radius = child->IntAttribute("r");
                svg_elements.push_back(new Circle(fill, center, radius));
            }
            else if (name == "line")
            {
            
                const char* stroke_attr = child->Attribute("stroke");
                Color stroke = stroke_attr ? parse_color(stroke_attr) : Color{0, 0, 0};
                Point p1 = { child->IntAttribute("x1"), child->IntAttribute("y1") };
                Point p2 = { child->IntAttribute("x2"), child->IntAttribute("y2") };
                svg_elements.push_back(new Line(stroke, p1, p2));
            }
            else if (name == "rect")
            {
                Color fill = parse_color(child->Attribute("fill"));
                Point topLeft = { child->IntAttribute("x"), child->IntAttribute("y") };
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                svg_elements.push_back(new Rect(fill, topLeft, width, height));
            }
            else if (name == "polygon")
            {
                // Polígonos usam estritamente 'fill'
                const char* fill_attr = child->Attribute("fill");
                Color fill = fill_attr ? parse_color(fill_attr) : Color{0, 0, 0};
                std::vector<Point> points = parse_points(child->Attribute("points"));
                svg_elements.push_back(new Polygon(fill, points));
            }
            else if (name == "polyline")
            {
                const char* stroke_attr = child->Attribute("stroke");
                Color stroke = stroke_attr ? parse_color(stroke_attr) : Color{0, 0, 0};
                std::vector<Point> points = parse_points(child->Attribute("points"));
                svg_elements.push_back(new Polyline(stroke, points));
            }



        }

        
    }
    
}
