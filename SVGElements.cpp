#include "SVGElements.hpp"
#include <sstream>

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement(const std::string &transform, const Point &origin)
        : transform_(transform), transform_origin(origin) {} //3
    //contrtutor


    SVGElement::~SVGElement() {}
    //destrutor


    //3
Point SVGElement::apply_transforms(Point p) const
    {
        if (transform_.empty()) {
            return p;
        }

        size_t open_bracket = transform_.find('(');
        size_t close_bracket = transform_.find(')');
         //procura na string o parêntesis de abertura e fecho, para o transform
        
        if (open_bracket == std::string::npos || close_bracket == std::string::npos) {
            return p;
        }
        // se não encontrar retorna p, que são as cordenadas do que seria transformado

        std::string tipo = transform_.substr(0, open_bracket);
        // retira tudo o que está antes do 1 parêntesis, ou seja, o tipo de transformação que irá ocorrer
        
        std::string argumento = transform_.substr(open_bracket + 1, close_bracket - open_bracket - 1);
        // agora retira apenas os dados
        
        
        for (char &c : argumento) {
            if (c == ',') c = ' ';
        }
        // substitui as vírgulas por espaços, para que os dados sejam usados no s_tr_ing_stre_am
        
        std::stringstream ss(argumento);
        //cria um novo ss para desenvolver um novo fluxo, entre os dados

        if (tipo == "translate")
        { // se for deste tipo.... 
            int tx = 0, ty = 0;
            if (ss >> tx >> ty) { // do fluxo enunciado extrai as duas variáveis e transforma-as
                p = p.translate({tx, ty});
            }
        }
        else if (tipo == "rotate")
        {
            int graus = 0;
            if (ss >> graus) {
                p = p.rotate(transform_origin, graus);
            }
        }
        else if (tipo == "scale")
        {
            int v = 1;
            if (ss >> v) {
                p = p.scale(transform_origin, v);
            }
        }
        return p;
    }

    //3

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius,const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        // 3
        Point transformed_center = apply_transforms(center);
    
        Point edge_x = { center.x + radius.x, center.y };
        Point edge_y = { center.x, center.y + radius.y };
    
        Point transformed_edge_x = apply_transforms(edge_x);
        Point transformed_edge_y = apply_transforms(edge_y);
    
        int new_rx = std::abs(transformed_edge_x.x - transformed_center.x);
        int new_ry = std::abs(transformed_edge_y.y - transformed_center.y);
    
        if (new_rx == 0) new_rx = std::abs(transformed_edge_x.y - transformed_center.y);
        if (new_ry == 0) new_ry = std::abs(transformed_edge_y.x - transformed_center.x);

        img.draw_ellipse(transformed_center, {new_rx, new_ry}, fill);
        //3
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->
//escrevemos:
Circle::Circle(const Color& fill,
               const Point& center,
               const int& radius,
                const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        fill(fill), center(center), radius(radius)
{}
void Circle::draw(PNGImage &img) const
    {
        Point transformed_center = apply_transforms(center);
        Point edge_point = { center.x + radius, center.y };
        Point transformed_edge = apply_transforms(edge_point);
        int new_radius = std::abs(transformed_edge.x - transformed_center.x);
        if (new_radius == 0) {
            new_radius = std::abs(transformed_edge.y - transformed_center.y);
        }

        img.draw_ellipse(transformed_center, {new_radius, new_radius}, fill);
    }
//escrevemos:
Polyline::Polyline(const Color& stroke,
                   const std::vector<Point>& points,
                const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        stroke(stroke),
        points(points)
{
}

void Polyline::draw(PNGImage& img) const
{
    for(size_t i = 1; i < points.size(); i++)
    {
        img.draw_line(apply_transforms(points[i - 1]), 
                    apply_transforms(points[i]), 
                    stroke);
    }
}

//escrevemos:
Line::Line(const Color& stroke,
           const Point& p1,
           const Point& p2,
        const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        stroke(stroke), p1(p1), p2(p2) {}
void Line::draw(PNGImage &img) const
    {
        img.draw_line(apply_transforms(p1), apply_transforms(p2), stroke);
    }
//escrevemos:
Polygon::Polygon(const Color& fill,
                 const std::vector<Point>& points, 
                const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        fill(fill), points(points)
{
}

void Polygon::draw(PNGImage& img) const
{
    //3 - transformar o vetor
    std::vector<Point> transformed_pts;
    transformed_pts.reserve(points.size());
    for (const Point &p : points) {
        transformed_pts.push_back(apply_transforms(p));
    }
    img.draw_polygon(transformed_pts, fill);
    //3
}
//escrevemos:
Rect::Rect(const Color& fill,
           const Point& top_left,
           int width,
           int height, const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
           fill(fill), top_left(top_left), width(width), height(height) {}
void Rect::draw(PNGImage &img) const
    {
        
        std::vector<Point> pts = {
            top_left,
            {top_left.x + width - 1, top_left.y},
            {top_left.x + width - 1, top_left.y + height - 1},
            {top_left.x, top_left.y + height - 1}
        };
        //3 - transformar o vetor
        std::vector<Point> transformed_pts;
        transformed_pts.reserve(4);
        for (const Point &p : pts) {
            transformed_pts.push_back(apply_transforms(p));
        }
        img.draw_polygon(transformed_pts, fill);
        //3
    }

}
