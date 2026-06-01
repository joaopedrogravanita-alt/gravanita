#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->
//escrevemos:
Circle::Circle(const Color& fill,
               const Point& center,
               const int& radius)
    : fill(fill), center(center), radius(radius)
{}
void Circle::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, {radius, radius}, fill);
    }
//escrevemos:
Polyline::Polyline(const Color& stroke,
                   const std::vector<Point>& points)
    : stroke(stroke),
      points(points)
{
}

void Polyline::draw(PNGImage& img) const
{
    for(size_t i = 1; i < points.size(); i++)
    {
        img.draw_line(points[i-1],
                      points[i],
                      stroke);
    }
}

//escrevemos:
Line::Line(const Color& stroke,
           const Point& p1,
           const Point& p2)
    : stroke(stroke), p1(p1), p2(p2) {}
void Line::draw(PNGImage &img) const
    {
        img.draw_line(p1, p2, stroke);
    }
//escrevemos:
Polygon::Polygon(const Color& fill,
                 const std::vector<Point>& points)
    : fill(fill),
      points(points)
{
}

void Polygon::draw(PNGImage& img) const
{
    img.draw_polygon(points, fill);
}
//escrevemos:
Rect::Rect(const Color& fill,
           const Point& topLeft,
           int width,
           int height): fill(fill), top_left(top_left), width(width), height(height) {}
void Rect::draw(PNGImage &img) const
    {
        // Mapeia os 4 cantos do retângulo (ajustando as dimensões com -1 para os limites do varrimento)
        std::vector<Point> pts = {
            top_left,
            {top_left.x + width - 1, top_left.y},
            {top_left.x + width - 1, top_left.y + height - 1},
            {top_left.x, top_left.y + height - 1}
        };
        img.draw_polygon(pts, fill);
    }

}
