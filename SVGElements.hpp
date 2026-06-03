#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <vector>
#include <string>
#include <cmath>

namespace svg
{
    struct Matrix {
        double m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
        static Matrix identity();
        static Matrix fromString(const std::string& transform, Point origin);
        Matrix multiply(const Matrix& other) const;
        Point apply(Point p) const;
    };

    class SVGElement
    {
    public:
        SVGElement(const std::string &transform = "", const Point &origin = {0, 0});
        virtual ~SVGElement();
        
        // New Matrix-based draw (primary)
        virtual void draw(PNGImage &img, Matrix currentMatrix) const = 0;
        
        // Backward-compatibility bridge for convert.cpp
        void draw(PNGImage &img) const;
        
        virtual SVGElement* clone() const = 0; 
        virtual void set_transform(const std::string& t) { transform_ = t; }
        virtual std::string get_transform() const { return transform_; }

    protected:
        std::string transform_;
        Point transform_origin;
    };

    // ... (Remaining class declarations for Ellipse, Circle, etc., remain unchanged)
    class Ellipse : public SVGElement {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius, const std::string &transform = "", const Point &origin = {0, 0});
        void draw(PNGImage &img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
    private:
        Color fill; Point center; Point radius;
    };

    class Circle : public SVGElement {
    public:
        Circle(const Color &fill, const Point &center, const int &radius, const std::string &transform = "", const Point &origin = {0, 0});
        void draw(PNGImage &img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
    private:
        Color fill; Point center; int radius;
    };

    class Polyline : public SVGElement {
    public:
        Polyline(const Color& stroke, const std::vector<Point>& points, const std::string &transform = "", const Point &origin = {0, 0});
        void draw(PNGImage& img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
    private:
        Color stroke; std::vector<Point> points;
    };

    class Line : public SVGElement {
    public:
        Line(const Color& stroke, const Point& p1, const Point& p2, const std::string &transform = "", const Point &origin = {0, 0});
        void draw(PNGImage &img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
    private:
        Color stroke; Point p1; Point p2;
    };

    class Polygon : public SVGElement {
    public:
        Polygon(const Color& fill, const std::vector<Point>& points, const std::string &transform = "", const Point &origin = {0, 0});
        void draw(PNGImage& img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
    private:
        Color fill; std::vector<Point> points;
    };

    class Rect : public SVGElement {
    public:
        Rect(const Color& fill, const Point& top_left, int width, int height, const std::string &transform = "", const Point &origin = {0, 0});
        void draw(PNGImage &img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
    private:
        Color fill; Point top_left; int width; int height;
    };

    class Group : public SVGElement {
    public:
        Group(const std::string &transform = "", const Point &origin = {0, 0});
        Group(const std::vector<SVGElement*> &elements, const std::string &transform = "", const Point &origin = {0, 0});
        ~Group() override;
        void draw(PNGImage& img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
        void addElement(SVGElement* elem);
    private:
        std::vector<SVGElement*> elements;
    };

    class Use : public SVGElement {
    public:
        Use(SVGElement* elem, const std::string &transform = "", const Point &origin = {0, 0});
        ~Use() override;
        void draw(PNGImage& img, Matrix currentMatrix) const override;
        SVGElement* clone() const override;
    private:
        SVGElement* clonedElement;
    };

    void readSVG(const std::string &svg_file, Point &dimensions, std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file, const std::string &png_file);
} // namespace svg
#endif