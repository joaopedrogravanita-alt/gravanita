
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


    using namespace tinyxml2;
    
    // Função auxiliar para ler a string de pontos (usada em Polyline e Polygon)
    std::vector<Point> parsePoints(const std::string& pointsStr) {
        std::vector<Point> points;
        std::string cleanStr = pointsStr;
        // Substitui vírgulas por espaços para facilitar a extração com stringstream
        for (char& c : cleanStr) {
            if (c == ',') c = ' ';
        }
        std::stringstream ss(cleanStr);
        int x, y;
        while (ss >> x >> y) {
            points.push_back({x, y});
        }
        return points;
    }
    
    // Função auxiliar para ler o transform-origin (converte "X Y" para Point)
    Point parseOrigin(const char* attrStr) {
        if (!attrStr) return {0, 0};
        std::stringstream ss(attrStr);
        int x = 0, y = 0;
        ss >> x >> y;
        return {x, y};
    }
    
    // Função auxiliar/recursiva para ler qualquer elemento SVG
    SVGElement* readElement(XMLElement* elem, std::map<std::string, SVGElement*>& idMap) {
        std::string name = elem->Value();
        SVGElement* svgElem = nullptr;
    
        // Lendo os atributos de transformação comuns a todas as formas
        std::string transform = elem->Attribute("transform") ? elem->Attribute("transform") : "";
        Point transformOrigin = parseOrigin(elem->Attribute("transform-origin"));
    
        // 1. LEITURA DA ELLIPSE
        if (name == "ellipse") {
            Color fill = Color::parse(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            Point center = { elem->IntAttribute("cx"), elem->IntAttribute("cy") };
            Point radius = { elem->IntAttribute("rx"), elem->IntAttribute("ry") };
            svgElem = new Ellipse(fill, center, radius, transform, transformOrigin);
        }
        // 2. LEITURA DO CIRCLE
        else if (name == "circle") {
            Color fill = Color::parse(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            Point center = { elem->IntAttribute("cx"), elem->IntAttribute("cy") };
            int r = elem->IntAttribute("r");
            svgElem = new Circle(fill, center, r, transform, transformOrigin);
        }
        // 3. LEITURA DA POLYLINE
        else if (name == "polyline") {
            Color stroke = Color::parse(elem->Attribute("stroke") ? elem->Attribute("stroke") : "");
            std::vector<Point> points = parsePoints(elem->Attribute("points") ? elem->Attribute("points") : "");
            svgElem = new Polyline(stroke, points, transform, transformOrigin);
        }
        // 4. LEITURA DA LINE
        else if (name == "line") {
            Color stroke = Color::parse(elem->Attribute("stroke") ? elem->Attribute("stroke") : "");
            Point p1 = { elem->IntAttribute("x1"), elem->IntAttribute("y1") };
            Point p2 = { elem->IntAttribute("x2"), elem->IntAttribute("y2") };
            svgElem = new Line(stroke, p1, p2, transform, transformOrigin);
        }
        // 5. LEITURA DO POLYGON
        else if (name == "polygon") {
            Color fill = Color::parse(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            std::vector<Point> points = parsePoints(elem->Attribute("points") ? elem->Attribute("points") : "");
            svgElem = new Polygon(fill, points, transform, transformOrigin);
        }
        // 6. LEITURA DO RECT
        else if (name == "rect") {
            Color fill = Color::parse(elem->Attribute("fill") ? elem->Attribute("fill") : "");
            Point top_left = { elem->IntAttribute("x"), elem->IntAttribute("y") };
            int width = elem->IntAttribute("width");
            int height = elem->IntAttribute("height");
            svgElem = new Rect(fill, top_left, width, height, transform, transformOrigin);
        }
        // 7. LEITURA DO GRUPO <g>
        else if (name == "g") {
            Group* group = new Group(transform, transformOrigin);
    
            // Ciclo recursivo para colocar os elementos filhos dentro do grupo
            for (XMLElement* child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
                SVGElement* childElem = readElement(child, idMap); 
                if (childElem != nullptr) {
                    group->addElement(childElem);
                }
            }
            svgElem = group;
        }
        // 8. LEITURA DO DUPLICADO <use>
        else if (name == "use") {
            const char* hrefAttr = elem->Attribute("href");
            if (hrefAttr != nullptr && hrefAttr[0] == '#') {
                std::string targetId = hrefAttr + 1; // Remove o '#' para obter apenas o ID
    
                // Se o ID original existir no nosso mapa, fazemos um clone dele
                if (idMap.find(targetId) != idMap.end()) {
                    SVGElement* origin = idMap[targetId];
                    svgElem = new Use(origin->clone(), transform, transformOrigin);
                }
            }
        }
    
        // SE O ELEMENTO FOI CRIADO COM SUCESSO E TIVER UM ID, GUARDAMOS NO MAPA
        if (svgElem != nullptr) {
            const char* idAttr = elem->Attribute("id");
            if (idAttr != nullptr) {
                idMap[idAttr] = svgElem;
            }
        }
    
        return svgElem;
    }
    
    // Função principal chamada pelo motor do projeto
    void readSVG(const std::string &svg_file, Point &dimensions, std::vector<SVGElement *> &svg_elements) {
        XMLDocument doc;
        if (doc.LoadFile(svg_file.c_str()) != XML_SUCCESS) return;
    
        XMLElement* svgRoot = doc.FirstChildElement("svg");
        if (!svgRoot) return;
    
        // Guarda o tamanho da imagem final na referência passadas por argumento
        int width = svgRoot->IntAttribute("width");
        int height = svgRoot->IntAttribute("height");
        dimensions = { width, height };
    
        // Mapa para associar IDs textuais aos ponteiros correspondentes
        std::map<std::string, SVGElement*> idMap;
    
        // Percorre os nós principais imediatamente abaixo do nó <svg>
        for (XMLElement* elem = svgRoot->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
            SVGElement* svgElem = readElement(elem, idMap);
            if (svgElem != nullptr) {
                svg_elements.push_back(svgElem);
            }
        }
    }

} 
