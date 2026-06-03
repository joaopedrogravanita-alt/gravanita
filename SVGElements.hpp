//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <vector>
#include <string>

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement(const std::string &transform = "", const Point &origin = {0, 0});
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        
        //3
    protected:
        std::string transform_cmd;
        Point transform_origin;

        Point apply_transforms(Point p) const;
        //3
    };
    

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius, const std::string &transform = "", const Point &origin = {0, 0});
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };


//escrevemos:
class Circle : public SVGElement
{
public:
    Circle( const Color &fill, const Point &center, const int &radius, const std::string &transform = "", const Point &origin = {0, 0});
    void draw(PNGImage &img) const override;
private:
        Color fill;
        Point center;
        int radius;
};
//escrevemos:
class Polyline : public SVGElement
{
public:
    Polyline(const Color& stroke,
             const std::vector<Point>& points, const std::string &transform = "", const Point &origin = {0, 0});

    void draw(PNGImage& img) const override;

private:
    Color stroke;
    std::vector<Point> points;
};
//escrevemos:
class Line : public SVGElement
{
public:
    Line(const Color& stroke,
         const Point& p1,
         const Point& p2, const std::string &transform = "", const Point &origin = {0, 0});
    void draw(PNGImage &img) const override;
private:
        Color stroke;
        Point p1;
        Point p2;
};


//escrevemos:
class Polygon : public SVGElement
{
public:
    Polygon(const Color& fill,
            const std::vector<Point>& points, const std::string &transform = "", const Point &origin = {0, 0});

    void draw(PNGImage& img) const override;

private:
    Color fill;
    std::vector<Point> points;
};
//escrevemos:
class Rect : public SVGElement
{
public:
    Rect(const Color& fill,
         const Point& top_left,
         int width,
         int height, const std::string &transform = "", const Point &origin = {0, 0});
    void draw(PNGImage &img) const override;
private:
        Color fill;
        Point top_left;
        int width;
        int height;
};
}
#endif
