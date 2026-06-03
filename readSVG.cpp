
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <sstream>
#include <map>
#include <string>
#include "SVGElements.hpp"
#include "tinyxml2.h"

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
            const char* tipo = child->Attribute("transform");
            if (tipo != nullptr)
            {
                transform = tipo;
            }

            Point transform_origin = {0, 0};
            const char* argumento = child->Attribute("transform-origin");
            if (argumento != nullptr)
            {
                stringstream ss(argumento);
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
                svg_elements.push_back(new Circle(fill, center, radius, transform, transform_origin));//3
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
                svg_elements.push_back(new Rect(fill, topLeft, width, height,  transform, transform_origin)); //3
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
BRUNO: VE ISTO AQUI
using namespace tinyxml2;

// Função auxiliar/recursiva para ler qualquer elemento SVG
SVGElement* readElement(XMLElement* elem, std::map<std::string, SVGElement*>& idMap) {
    std::string name = elem->Value();
    SVGElement* svgElem = nullptr;

    if (name == "circle") {
        // ... (teu código atual para ler círculo) ...
        // svgElem = new Circle(...);
    }
    else if (name == "rect") {
        // ... (teu código atual para ler retângulo) ...
    }
    // ==========================================
    // SE FOR UM GRUPO <g>
    // ==========================================
    else if (name == "g") {
        Group* group = new Group();

        // Ciclo para percorrer todos os nós filhos de <g>
        for (XMLElement* child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
            SVGElement* childElem = readElement(child, idMap); // Chamada recursiva!
            if (childElem != nullptr) {
                group->addElement(childElem);
            }
        }
        svgElem = group;
    }
    // ==========================================
    // SE FOR UM USE <use>
    // ==========================================
    else if (name == "use") {
        const char* hrefAttr = elem->Attribute("href");
        if (hrefAttr != nullptr && hrefAttr[0] == '#') {
            std::string targetId = hrefAttr + 1; // Ignora o caractere '#' para obter o ID limpo

            // Procura o ID no nosso mapa
            if (idMap.find(targetId) != idMap.end()) {
                // Se encontrou, clona o elemento original
                SVGElement* origin = idMap[targetId];
                svgElem = new Use(origin->clone());
            }
        }
    }

    // SE O ELEMENTO LIDO TIVER UM ID, GUARDAMOS NO MAPA
    if (svgElem != nullptr) {
        const char* idAttr = elem->Attribute("id");
        if (idAttr != nullptr) {
            idMap[idAttr] = svgElem;
        }
        
        // Aqui também vais ler os atributos comuns como o 'transform' do elemento atual
        // readTransform(elem, svgElem); 
    }

    return svgElem;
}

// Esta é a função principal que a Makefile chama
void readSVG(const std::string& filename, std::vector<SVGElement*>& rootElements) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) return;

    XMLElement* svgRoot = doc.FirstChildElement("svg");
    if (!svgRoot) return;

    // Criamos o mapa de IDs aqui no início
    std::map<std::string, SVGElement*> idMap;

    // Percorre os elementos principais do SVG
    for (XMLElement* elem = svgRoot->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        SVGElement* svgElem = readElement(elem, idMap);
        if (svgElem != nullptr) {
            rootElements.push_back(svgElem);
        }
    }
}
    
}
