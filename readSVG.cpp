
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <sstram>

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

            // 3
            string transform = "";
            const char* transform_attr = child->Attribute("transform");
            if (transform_attr != nullptr)
            {
                transform = transform_attr;
            }

            Point transform_origin = {0, 0};
            const char* origin_attr = child->Attribute("transform-origin");
            if (origin_attr != nullptr)
            {
                stringstream ss(origin_attr);
                ss >> transform_origin.x >> transform_origin.y;
            }

            //3

            if (name == "ellipse")
            {
                Color fill = parse_color(child->Attribute("fill"));
                Point center = { child->IntAttribute("cx"), child->IntAttribute("cy") };
                Point radius = { child->IntAttribute("rx"), child->IntAttribute("ry") };
                svg_elements.push_back(new Ellipse(fill, center, radius, transform, transform_origin)); //3
            }
            else if (name == "circle")
            {
                Color fill = parse_color(child->Attribute("fill"));
                Point center = { child->IntAttribute("cx"), child->IntAttribute("cy") };
                int radius = child->IntAttribute("r");
                svg_elements.push_back(new Circle(fill, center, radius, , transform, transform_origin));//3
            }
            else if (name == "line")
            {
            
                const char* stroke_attr = child->Attribute("stroke");
                Color stroke = stroke_attr ? parse_color(stroke_attr) : Color{0, 0, 0};
                Point p1 = { child->IntAttribute("x1"), child->IntAttribute("y1") };
                Point p2 = { child->IntAttribute("x2"), child->IntAttribute("y2") };
                svg_elements.push_back(new Line(stroke, p1, p2, transform, transform_origin)); //3
            }
            else if (name == "rect")
            {
                Color fill = parse_color(child->Attribute("fill"));
                Point topLeft = { child->IntAttribute("x"), child->IntAttribute("y") };
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                svg_elements.push_back(new Rect(fill, topLeft, width, height, , transform, transform_origin)); //3
            }
            else if (name == "polygon")
            {
                // Polígonos usam estritamente 'fill'
                const char* fill_attr = child->Attribute("fill");
                Color fill = fill_attr ? parse_color(fill_attr) : Color{0, 0, 0};
                const char* pts_attr = child->Attribute("points");
                if (pts_attr != nullptr)
                {
                    std::vector<Point> points;
                    std::stringstream ss(pts_attr);
                    std::string pair;
                    
                    while (ss >> pair)
                    {
                        size_t comma = pair.find(',');
                        if (comma != std::string::npos)
                        {
                            int px = std::stoi(pair.substr(0, comma));
                            int py = std::stoi(pair.substr(comma + 1));
                            points.push_back({px, py});
                        }
                    }
                    svg_elements.push_back(new Polygon(fill, points, transform, transform_origin));
                }
            }
            else if (name == "polyline")
            {
                const char* stroke_attr = child->Attribute("stroke");
                Color stroke = stroke_attr ? parse_color(stroke_attr) : Color{0, 0, 0};
                const char* pts_attr = child->Attribute("points");
                if (pts_attr != nullptr)
                {
                    std::vector<Point> points;
                    std::stringstream ss(pts_attr);
                    std::string pair;
                    
                    // Extrai e converte os pares x,y separados por espaços ou quebras de linha
                    while (ss >> pair)
                    {
                        size_t comma = pair.find(',');
                        if (comma != std::string::npos)
                        {
                            int px = std::stoi(pair.substr(0, comma));
                            int py = std::stoi(pair.substr(comma + 1));
                            points.push_back({px, py});
                        }
                    }
                    svg_elements.push_back(new Polyline(stroke, points , transform, transform_origin)); //3
                }
            }



        }

        
    }
    
}
