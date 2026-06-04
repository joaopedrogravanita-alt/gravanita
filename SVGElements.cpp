#include "SVGElements.hpp"
#include <sstream>
#include <cmath>
#include <algorithm>

namespace svg
{
    // Updated clamp to use dimensions passed from the main image object
    Point clamp(Point p, int w, int h) {
        return { std::max(0, std::min(w - 1, p.x)), std::max(0, std::min(h - 1, p.y)) };
    }

    //MATRIZ IMPLEMENTATIONS
    Matrix Matrix::identity() { return Matrix(); }

    Matrix Matrix::multiply(const Matrix& other) const {
        Matrix res;
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j) {
                res.m[i][j] = 0;
                for(int k = 0; k < 3; ++k) res.m[i][j] += m[i][k] * other.m[k][j];
            }
        return res;
    }

    Point Matrix::apply(Point p) const {
        double x = m[0][0] * p.x + m[0][1] * p.y + m[0][2];
        double y = m[1][0] * p.x + m[1][1] * p.y + m[1][2];
        return {(int)std::round(x), (int)std::round(y)};
    }

    Matrix Matrix::fromString(const std::string& transform, Point origin) {
        Matrix res = Matrix::identity();
        if (transform.empty()) return res;
        
        std::stringstream ss(transform);
        std::string task;
        while (std::getline(ss, task, ')')) {
            size_t open = task.find('(');
            if (open == std::string::npos) continue;
            std::string type = task.substr(0, open);
            type.erase(0, type.find_first_not_of(" \t\n\r"));
            std::string args = task.substr(open + 1);
            for(char &c : args) if(c == ',') c = ' ';
            std::stringstream ss_args(args);
            
            if (type == "translate") {
                double tx, ty; ss_args >> tx >> ty;
                Matrix t; t.m[0][2] = tx; t.m[1][2] = ty;
                res = res.multiply(t);
            } else if (type == "rotate") {
                double deg; ss_args >> deg;
                double rad = M_PI * deg / 180.0;
                Matrix r;
                r.m[0][0] = cos(rad); r.m[0][1] = -sin(rad); r.m[1][0] = sin(rad); r.m[1][1] = cos(rad);
                Matrix toOrig; toOrig.m[0][2] = -origin.x; toOrig.m[1][2] = -origin.y;
                Matrix fromOrig; fromOrig.m[0][2] = origin.x; fromOrig.m[1][2] = origin.y;
                res = res.multiply(fromOrig.multiply(r.multiply(toOrig)));
            } else if (type == "scale") {
                double s; ss_args >> s;
                Matrix sc; sc.m[0][0] = s; sc.m[1][1] = s;
                
                // Centered scale transformation
                Matrix toOrig; toOrig.m[0][2] = -origin.x; toOrig.m[1][2] = -origin.y;
                Matrix fromOrig; fromOrig.m[0][2] = origin.x; fromOrig.m[1][2] = origin.y;
                
                res = res.multiply(fromOrig.multiply(sc.multiply(toOrig)));
            }
        }
        return res;
    }

    //SVGELEMENT IMPLEMENTATIONS
    SVGElement::SVGElement(const std::string &transform, const Point &origin)
        : transform_(transform), transform_origin(origin) {}
    SVGElement::~SVGElement() {}

    void SVGElement::draw(PNGImage &img) const {
        this->draw(img, Matrix::identity());
    }

    SHAPE IMPLEMENTATIONS
    void Ellipse::draw(PNGImage &img, Matrix m) const {
        Matrix finalM = m.multiply(Matrix::fromString(transform_, transform_origin));
        img.draw_ellipse(clamp(finalM.apply(center), img.width(), img.height()), {radius.x, radius.y}, fill);
    }

    void Circle::draw(PNGImage &img, Matrix m) const {
        Matrix finalM = m.multiply(Matrix::fromString(transform_, transform_origin));
        img.draw_ellipse(clamp(finalM.apply(center), img.width(), img.height()), {radius, radius}, fill);
    }

    void Polyline::draw(PNGImage &img, Matrix m) const {
        Matrix finalM = m.multiply(Matrix::fromString(transform_, transform_origin));
        for (size_t i = 1; i < points.size(); i++) {
            img.draw_line(clamp(finalM.apply(points[i-1]), img.width(), img.height()), 
                          clamp(finalM.apply(points[i]), img.width(), img.height()), stroke);
        }
    }

    void Line::draw(PNGImage &img, Matrix m) const {
        Matrix finalM = m.multiply(Matrix::fromString(transform_, transform_origin));
        img.draw_line(clamp(finalM.apply(p1), img.width(), img.height()), 
                      clamp(finalM.apply(p2), img.width(), img.height()), stroke);
    }

    void Polygon::draw(PNGImage &img, Matrix m) const {
        Matrix finalM = m.multiply(Matrix::fromString(transform_, transform_origin));
        std::vector<Point> t_pts;
        for (const auto& p : points) t_pts.push_back(clamp(finalM.apply(p), img.width(), img.height()));
        img.draw_polygon(t_pts, fill);
    }

    void Rect::draw(PNGImage &img, Matrix m) const {
        Matrix finalM = m.multiply(Matrix::fromString(transform_, transform_origin));
        std::vector<Point> pts = {top_left, {top_left.x + width, top_left.y}, {top_left.x + width, top_left.y + height}, {top_left.x, top_left.y + height}};
        std::vector<Point> t_pts;
        for (const auto& p : pts) t_pts.push_back(clamp(finalM.apply(p), img.width(), img.height()));
        img.draw_polygon(t_pts, fill);
    }

    //GROUP AND USE
    Group::Group(const std::string &t, const Point &o) : SVGElement(t, o) {}
    Group::Group(const std::vector<SVGElement*> &elems, const std::string &t, const Point &o) : SVGElement(t, o), elements(elems) {}
    Group::~Group() { for (auto e : elements) delete e; }
    
    void Group::draw(PNGImage &img, Matrix m) const {
        Matrix localM = Matrix::fromString(transform_, transform_origin);
        Matrix groupM = m.multiply(localM); 
        for (auto child : elements) child->draw(img, groupM);
    }
    
    SVGElement* Group::clone() const {
        std::vector<SVGElement*> cloned;
        for (auto e : elements) cloned.push_back(e->clone());
        return new Group(cloned, transform_, transform_origin);
    }
    void Group::addElement(SVGElement* e) { elements.push_back(e); }

    Use::Use(SVGElement* elem, const std::string &t, const Point &o) : SVGElement(t, o), clonedElement(elem) {}
    Use::~Use() { delete clonedElement; }
    
    void Use::draw(PNGImage &img, Matrix m) const {
        Matrix useM = m.multiply(Matrix::fromString(transform_, transform_origin));
        clonedElement->draw(img, useM);
    }
    SVGElement* Use::clone() const { return new Use(clonedElement->clone(), transform_, transform_origin); }

    //CONSTRUCTORS
    Ellipse::Ellipse(const Color &f, const Point &c, const Point &r, const std::string &t, const Point &o) : SVGElement(t, o), fill(f), center(c), radius(r) {}
    SVGElement* Ellipse::clone() const { return new Ellipse(fill, center, radius, transform_, transform_origin); }
    Circle::Circle(const Color &f, const Point &c, const int &r, const std::string &t, const Point &o) : SVGElement(t, o), fill(f), center(c), radius(r) {}
    SVGElement* Circle::clone() const { return new Circle(fill, center, radius, transform_, transform_origin); }
    Polyline::Polyline(const Color& s, const std::vector<Point>& p, const std::string &t, const Point &o) : SVGElement(t, o), stroke(s), points(p) {}
    SVGElement* Polyline::clone() const { return new Polyline(stroke, points, transform_, transform_origin); }
    Line::Line(const Color& s, const Point& p1, const Point& p2, const std::string &t, const Point &o) : SVGElement(t, o), stroke(s), p1(p1), p2(p2) {}
    SVGElement* Line::clone() const { return new Line(stroke, p1, p2, transform_, transform_origin); }
    Polygon::Polygon(const Color& f, const std::vector<Point>& p, const std::string &t, const Point &o) : SVGElement(t, o), fill(f), points(p) {}
    SVGElement* Polygon::clone() const { return new Polygon(fill, points, transform_, transform_origin); }
    Rect::Rect(const Color& f, const Point& tl, int w, int h, const std::string &t, const Point &o) : SVGElement(t, o), fill(f), top_left(tl), width(w), height(h) {}
    SVGElement* Rect::clone() const { return new Rect(fill, top_left, width, height, transform_, transform_origin); }
}