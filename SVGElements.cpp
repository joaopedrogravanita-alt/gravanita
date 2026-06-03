#include "SVGElements.hpp"
#include <sstream>

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement(const std::string &transform, const Point &origin)
        : transform_cmd(transform), transform_origin(origin) {} //3
    SVGElement::~SVGElement() {}


    //3
Point SVGElement::apply_transforms(Point p) const
    {
        if (transform_cmd.empty()) {
            return p;
        }

        size_t open_bracket = transform_cmd.find('(');
        size_t close_bracket = transform_cmd.find(')');
        if (open_bracket == std::string::npos || close_bracket == std::string::npos) {
            return p;
        }

        std::string type = transform_cmd.substr(0, open_bracket);
        std::string args_str = transform_cmd.substr(open_bracket + 1, close_bracket - open_bracket - 1);
        
        // Normaliza separadores substituindo vírgulas por espaços
        for (char &c : args_str) {
            if (c == ',') c = ' ';
        }
        
        std::stringstream ss(args_str);

        if (type == "translate")
        {
            int tx = 0, ty = 0;
            if (ss >> tx >> ty) {
                p = p.translate({tx, ty});
            }
        }
        else if (type == "rotate")
        {
            int degrees = 0;
            if (ss >> degrees) {
                p = p.rotate(transform_origin, degrees);
            }
        }
        else if (type == "scale")
        {
            int factor = 1;
            if (ss >> factor) {
                p = p.scale(transform_origin, factor);
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
        // Mapeia os 4 cantos do retângulo (ajustando as dimensões com -1 para os limites do varrimento)
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
